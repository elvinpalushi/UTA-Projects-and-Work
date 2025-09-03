import sys
import numpy as np
import skimage.io as io
import matplotlib.pyplot as plt

def rgb_to_hsv(img):
    img = img / 255.0
    R, G, B = img[..., 0], img[..., 1], img[..., 2]

    V = np.max(img, axis=2)
    C = V - np.min(img, axis=2)
    S = np.where(V == 0, 0, C / V)

    H = np.where(C == 0, 0, C)
    H = np.where(V == R, ((G - B) / H) % 6, H)
    H = np.where(V == G, ((B - R) / H) + 2, H)
    H = np.where(V == B, ((R - G) / H) + 4, H)

    H = (H * 60)

    return np.stack([H, S, V], axis=2)

def hsv_to_rgb(img):
    H, S, V = img[..., 0], img[..., 1], img[..., 2]
    C = V * S
    H = H / 60
    X = C * (1 - np.abs((H % 2) - 1))

    R_prime, G_prime, B_prime = np.zeros_like(H), np.zeros_like(H), np.zeros_like(H)

    R_prime = np.where((0 <= H) & (H < 1), C, R_prime)
    G_prime = np.where((0 <= H) & (H < 1), X, G_prime)
    B_prime = np.where((0 <= H) & (H < 1), 0, B_prime)

    R_prime = np.where((1 <= H) & (H < 2), X, R_prime)
    G_prime = np.where((1 <= H) & (H < 2), C, G_prime)
    B_prime = np.where((1 <= H) & (H < 2), 0, B_prime)

    R_prime = np.where((2 <= H) & (H < 3), 0, R_prime)
    G_prime = np.where((2 <= H) & (H < 3), C, G_prime)
    B_prime = np.where((2 <= H) & (H < 3), X, B_prime)

    R_prime = np.where((3 <= H) & (H < 4), 0, R_prime)
    G_prime = np.where((3 <= H) & (H < 4), X, G_prime)
    B_prime = np.where((3 <= H) & (H < 4), C, B_prime)

    R_prime = np.where((4 <= H) & (H < 5), X, R_prime)
    G_prime = np.where((4 <= H) & (H < 5), 0, G_prime)
    B_prime = np.where((4 <= H) & (H < 5), C, B_prime)

    R_prime = np.where((5 <= H) & (H < 6), C, R_prime)
    G_prime = np.where((5 <= H) & (H < 6), 0, G_prime)
    B_prime = np.where((5 <= H) & (H < 6), X, B_prime)

    m = V - C

    R, G, B = (R_prime + m), (G_prime + m), (B_prime + m)
    return np.stack([R, G, B], axis=2)

def main():
    if len(sys.argv) != 5:
        print("Give filename, hue_mod, saturation_mod, and value_mod on command line.")
        sys.exit(1)

    filename = sys.argv[1]
    
    try:
        hue_mod = float(sys.argv[2])
        sat_mod = float(sys.argv[3])
        val_mod = float(sys.argv[4])
    except ValueError:
        print("HSV Modifications must be numbers.")
        sys.exit(1)

    hue_mod = max(0, min(hue_mod, 360))

    if not (0 <= sat_mod <= 1) or not (0 <= val_mod <= 1):
        print("Saturation and Value inputs should be within range [0, 1].")
        sys.exit(1)

    try:
        img = io.imread(filename)
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
        sys.exit(1)

    img_np = np.array(img)

    hsv_img = rgb_to_hsv(img_np)
    hsv_img[np.isnan(hsv_img)] = 0.
    
    # Uncomment lines 93 and 99 if you want to give hue value from [0,1] to simulate something like skimage.color
    # hsv_img[:, :, 0] = hsv_img[:, :, 0] / 360.0

    hsv_img[:, :, 0] += hue_mod
    hsv_img[:, :, 1] += sat_mod
    hsv_img[:, :, 2] += val_mod

    # hsv_img[:, :, 0] = hsv_img[:, :, 0] * 360.0

    hsv_img[:, :, 0] %= 360.0

    mod_img = hsv_to_rgb(hsv_img)

    plt.figure(figsize=(10, 5))
    plt.subplot(1, 2, 1)
    plt.title('Original Image')
    plt.imshow(img)
    plt.axis('off')

    plt.subplot(1, 2, 2)
    plt.title('Modified Image')
    plt.imshow(mod_img)
    plt.axis('off')

    plt.show()

if __name__ == "__main__":
    main()
