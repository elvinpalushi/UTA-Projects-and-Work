import torch
from torch import nn
import torch.nn.functional as F
import lightning as L
import torchmetrics
from torchvision import transforms
from torchvision.datasets import Imagenette
from lightning.pytorch.callbacks.early_stopping import EarlyStopping
from lightning.pytorch.callbacks import ModelCheckpoint

class AllConvolutionalNet(L.LightningModule):
    def __init__(self, num_classes=10):
        super().__init__()
        
        self.features = nn.Sequential(
            # Input is 160x160x3
            nn.Conv2d(3, 32, 5),
            nn.ReLU(),
            nn.Conv2d(32, 64, 5, stride=2),
            nn.ReLU(),
            nn.Conv2d(64, num_classes, 5),
            nn.ReLU(),
        )
        
        self.gap = nn.AdaptiveAvgPool2d(1)

        self.accuracy = torchmetrics.Accuracy(task="multiclass", num_classes=num_classes)

    def forward(self, x):
        x = self.features(x)
        x = self.gap(x)

        x = x.view(x.shape[0], -1)

        return x

    def training_step(self, batch, batch_idx):
        x, y = batch
        y_hat = self(x)
        loss = F.cross_entropy(y_hat, y)

        self.log("train_loss", loss)
        return loss

    def validation_step(self, batch, batch_idx):
        x, y = batch
        y_hat = self(x)
        loss = F.cross_entropy(y_hat, y)

        self.accuracy(y_hat, y)

        self.log("val_accuracy", self.accuracy)
        self.log("val_loss", loss)

    def test_step(self, batch, batch_idx):
        x, y = batch
        y_hat = self(x)
        loss = F.cross_entropy(y_hat, y)

        self.accuracy(y_hat, y)

        self.log("test_accuracy", self.accuracy)
        self.log("test_loss", loss)

    def configure_optimizers(self):
        optimizer = torch.optim.Adam(self.parameters(), lr=1e-3)
        return optimizer

def main():
    # Prepare the dataset
    train_transforms = transforms.Compose([
        transforms.Resize(160),
        transforms.CenterCrop(160),
        transforms.ToTensor(),
        transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2470, 0.2435, 0.2616)),
    ])

    test_transforms = transforms.Compose([
        transforms.Resize(160),
        transforms.CenterCrop(160),
        transforms.ToTensor(),
        transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2470, 0.2435, 0.2616)),
    ])

    train_dataset = Imagenette("data/imagenette/train/", split="train", size="160px", download=True, transform=train_transforms)

    # Use 10% of the training set for validation
    train_set_size = int(len(train_dataset) * 0.9)
    val_set_size = len(train_dataset) - train_set_size

    seed = torch.Generator().manual_seed(42)
    train_dataset, val_dataset = torch.utils.data.random_split(train_dataset, [train_set_size, val_set_size], generator=seed)
    val_dataset.dataset.transform = test_transforms

    # Use DataLoader to load the dataset
    train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=32, num_workers=4, shuffle=True, persistent_workers=True)
    val_loader = torch.utils.data.DataLoader(val_dataset, batch_size=32, num_workers=4, shuffle=False, persistent_workers=True)

    # Configure the test dataset
    test_dataset = Imagenette("data/imagenette/test/", split="val", size="160px", download=True, transform=test_transforms)

    model = AllConvolutionalNet()

    # Add EarlyStopping
    early_stop_callback = EarlyStopping(monitor="val_loss",
                                        mode="min",
                                        patience=5)

    # Configure Checkpoints
    checkpoint_callback = ModelCheckpoint(
        monitor="val_loss",
        mode="min"
    )
    # Fit the model
    trainer = L.Trainer(callbacks=[early_stop_callback, checkpoint_callback], logger=True)
    trainer.fit(model=model, train_dataloaders=train_loader, val_dataloaders=val_loader)
    # Evaluate the model on the test set
    test_loader = torch.utils.data.DataLoader(test_dataset, batch_size=32, num_workers=4, shuffle=False, persistent_workers=True)
    trainer.test(model=model, dataloaders=test_loader)

if __name__ == '__main__':
    main()