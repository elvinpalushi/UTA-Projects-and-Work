import numpy as np
from skimage.feature import SIFT, hog
from skimage.color import rgb2gray
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
from tqdm import tqdm
from matplotlib.patches import ConnectionPatch

def match_desc(descriptors1, descriptors2):
    matches = []
    
    # Keep track of matches and use L2 distance to find the best matches
    for i, desc1 in enumerate(descriptors1):
        distance = np.linalg.norm(desc1 - descriptors2, axis=1)
        j = np.argmin(distance)
        matches.append((i,j))
    return np.array(matches)

def plot_keypoint_match(img1, img2, keypoints1, keypoints2, matches):
    # Select the points in img1 that match with img2 and vice versa
    dst = keypoints1[matches[:, 0]]
    src = keypoints2[matches[:, 1]]

    fig = plt.figure(figsize=(8, 4))
    ax1 = fig.add_subplot(121)
    ax2 = fig.add_subplot(122)
    ax1.imshow(img1, cmap='gray')
    ax2.imshow(img2, cmap='gray')

    for i in range(src.shape[0]):
        coordB = [dst[i, 1], dst[i, 0]]
        coordA = [src[i, 1], src[i, 0]]
        con = ConnectionPatch(xyA=coordA, xyB=coordB, coordsA="data", coordsB="data",
                              axesA=ax2, axesB=ax1, color="red")
        ax2.add_artist(con)
        ax1.plot(dst[i, 1], dst[i, 0], 'ro')
        ax2.plot(src[i, 1], src[i, 0], 'ro')
    plt.show()

def extract_sift_features(images, y_value):
    sift = SIFT()
    sift_features = []
    y_features = []

    for idx, image in enumerate(tqdm(images, desc="Extracting SIFT features")):
        try:
            sift.detect_and_extract(image)
            sift_features.append(sift.descriptors)
            y_features.append(y_value[idx])
        except:
            pass
    
    # Print the total number of SIFT features extracted
    total_sift_features = sum(len(features) for features in sift_features)
    print(f"Total SIFT features extracted: {total_sift_features}")

    sift_features_np = np.concatenate(sift_features)

    # Create a KMeans model to cluster the SIFT features
    vocab_size = 100
    kmeans = KMeans(n_clusters=vocab_size, random_state=42)

    # Fit the KMeans model to the SIFT features
    kmeans.fit(sift_features_np)

    image_histograms = []

    for feature in tqdm(sift_features, desc="Building histograms"):
        # Predict the closest cluster for each feature
        clusters = kmeans.predict(feature)
        # Build a histogram of the clusters
        histogram, _ = np.histogram(clusters, bins=vocab_size, range=(0, vocab_size))
        image_histograms.append(histogram)

    # Convert the list of histograms to a numpy array
    image_histograms_np = np.array(image_histograms)
    
    return image_histograms_np, np.array(y_features)

def extract_hog_features(images):
    hog_features = []
    for image in tqdm(images, desc="Extracting HOG features"):
        try:
            fd = hog(image, pixels_per_cell=(8, 8), cells_per_block=(2, 2), visualize=False)
            hog_features.append(fd)
        except:
            pass
    return np.array(hog_features)

if __name__ == "__main__":
    # Load the pre-split data
    data = np.load("cifar10.npz", allow_pickle=True)

    # Extract features from the training data
    X_train = data["X_train"].astype(np.uint8)
    y_train = data["y_train"].astype(np.uint8)
    X_test = data["X_test"].astype(np.uint8)
    y_test = data["y_test"].astype(np.uint8)

    # Reshape the data to (num_images, height, width, num_channels)
    X_train_rgb = X_train.reshape(-1, 3, 32, 32).transpose(0, 2, 3, 1)
    X_test_rgb = X_test.reshape(-1, 3, 32, 32).transpose(0, 2, 3, 1)

    # Convert the images to grayscale
    X_train_gray = rgb2gray(X_train_rgb)
    X_test_gray = rgb2gray(X_test_rgb)

    # Extract SIFT features
    sift_features_train, sift_y_train = extract_sift_features(X_train_gray, y_train)
    sift_features_test, sift_y_test = extract_sift_features(X_test_gray, y_test)

    # Extract HOG features
    hog_features_train = extract_hog_features(X_train_gray)
    hog_features_test = extract_hog_features(X_test_gray)

    sift = SIFT()

    # Detect SIFT keypoints and descriptors for two images
    sift.detect_and_extract(X_train_gray[1])
    keypoints1 = sift.keypoints
    descriptors1 = sift.descriptors

    sift.detect_and_extract(X_train_gray[1])
    keypoints2 = sift.keypoints
    descriptors2 = sift.descriptors

    # Match descriptors between the two images
    matches = match_desc(descriptors1, descriptors2)

    # Plot the keypoint matches
    plot_keypoint_match(X_train_gray[1], X_train_gray[1], keypoints1, keypoints2, matches)

    # Save the extracted features to files
    np.savez("sift_features.npz", X_train=sift_features_train, y_train=sift_y_train, X_test=sift_features_test, y_test=sift_y_test)
    np.savez("hog_features.npz", X_train=hog_features_train, y_train=y_train, X_test=hog_features_test, y_test=y_test)

    print("Feature extraction complete. Files saved: sift_features.npz, hog_features.npz")