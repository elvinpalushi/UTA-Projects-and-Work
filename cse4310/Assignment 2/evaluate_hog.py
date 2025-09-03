# Train an SVM classifier
import numpy as np
from sklearn.svm import LinearSVC

# Load the HOG features
data = np.load("hog_features.npz", allow_pickle=True)

X_train = data["X_train"]
y_train = data["y_train"]
X_test = data["X_test"]
y_test = data["y_test"]

# Create an SVM model
svm = LinearSVC(random_state=42)

# Train the model
svm.fit(X_train, y_train)

# Predict labels for the test set
y_pred = svm.predict(X_test)

# Calculate the number of correct predictions
correct_predictions = np.sum(y_pred == y_test)
total_samples = y_test.shape[0]

# Evaluate the model
accuracy = svm.score(X_test, y_test)
print(f'SVM accuracy: {accuracy:.2f}')
print(f'Number of correct predictions: {correct_predictions} out of {total_samples}')