import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.models as models
from torchvision import transforms
from torchvision.datasets import CIFAR10
from lightning.pytorch.callbacks import ModelCheckpoint
import lightning as L
import torch.nn.functional as F
import torchmetrics
from lightning.pytorch.callbacks.early_stopping import EarlyStopping


class RegularizationCNN(L.LightningModule):
    def __init__(self, num_classes=10):
        super().__init__()
        
        self.features = nn.Sequential(
            # Input is 160x160x3
            nn.Conv2d(3, 32, 5, padding=2),
            nn.ReLU(),
            nn.MaxPool2d(2),
            
            nn.Conv2d(32, 64, 5, padding=2),
            nn.ReLU(),
            nn.MaxPool2d(2),
            
            nn.Conv2d(64, 128, 5, padding=2),
            nn.ReLU(),
            nn.MaxPool2d(2),
        )
        
        self.estimator = nn.Sequential(
            nn.Linear(128 * 20 * 20, 512),
            nn.ReLU(),
            nn.Linear(512, num_classes)
        )

    def forward(self, x):
        x = self.features(x)
        x = x.view(x.shape[0], -1)

        return self.estimator(x)

def create_datasets(data_path):
    train_transforms = transforms.Compose([
        transforms.Resize(160),
        transforms.CenterCrop(160),
        transforms.ToTensor(),
        transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2470, 0.2435, 0.2616)),
    ])
    
    # Validation/test transforms
    test_transforms = transforms.Compose([
        transforms.Resize(160),
        transforms.CenterCrop(160),
        transforms.ToTensor(),
        transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2470, 0.2435, 0.2616)),
    ])
    

    train_dataset = CIFAR10(data_path, train=True, transform=train_transforms, download=True)
    test_dataset = CIFAR10(data_path, train=False, transform=test_transforms, download=True)
    
    train_set_size = int(0.9 * len(train_dataset))
    val_set_size = len(train_dataset) - train_set_size
    train_dataset, val_dataset = torch.utils.data.random_split(train_dataset, [train_set_size, val_set_size])
    
    val_dataset.dataset.transform = test_transforms
    
    return train_dataset, val_dataset, test_dataset

def create_data_loaders(data_path, batch_size=32):
    train_dataset, val_dataset, test_dataset = create_datasets(data_path)
    
    train_loader = torch.utils.data.DataLoader(
        train_dataset,
        batch_size=batch_size,
        num_workers=4,
        shuffle=True,
        persistent_workers=True
    )
    
    val_loader = torch.utils.data.DataLoader(
        val_dataset,
        batch_size=batch_size,
        num_workers=4,
        shuffle=False,
        persistent_workers=True
    )
    
    test_loader = torch.utils.data.DataLoader(
        test_dataset,
        batch_size=batch_size,
        num_workers=4,
        shuffle=False,
        persistent_workers=True
    )
    
    return train_loader, val_loader, test_loader

class TransferLearning(L.LightningModule):
    def __init__(self, lr):
        super().__init__()
        
        self.lr = lr

        self.model = RegularizationCNN()

        num_target_classes = 10

        state_dict = torch.load("imagenette_model.pth")
        self.model.load_state_dict(state_dict)

        self.accuracy = torchmetrics.Accuracy(task="multiclass", num_classes=num_target_classes)

    def forward(self, x):
        return self.model(x)
        
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
        return optim.Adam(self.parameters(), lr=self.lr)

def main():
    # Add EarlyStopping
    early_stop_callback = EarlyStopping(monitor="val_loss",
                                        mode="min",
                                        patience=5)

    # Configure Checkpoints
    checkpoint_callback = ModelCheckpoint(
        monitor="val_loss",
        mode="min"
    )

    train_loader, val_loader, test_loader = create_data_loaders("data/cifar10", batch_size=32)

    model = TransferLearning(1e-3)

    trainer = L.Trainer(callbacks=[early_stop_callback, checkpoint_callback], logger=True)
    trainer.fit(model=model, train_dataloaders=train_loader, val_dataloaders=val_loader)

    trainer.test(model=model, dataloaders=test_loader)

    print(model.state_dict())

if __name__ == '__main__':
    main()