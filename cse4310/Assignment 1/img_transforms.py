import numpy as np
import random
import sys
import skimage.io as io
import matplotlib.pyplot as plt
from numpy.lib import stride_tricks

def random_crop(img, size):
    h, w = img.shape[0], img.shape[1]

    if size <= 0 or size > min(h, w):
        raise ValueError(f"Crop size must be in the range (0, {min(h, w)}].")
    
    center_x = random.randint(size // 2, w - size // 2 - 1)
    center_y = random.randint(size // 2, h - size // 2 - 1)

    x_start = center_x - size // 2
    x_end = x_start + size
    y_start = center_y - size // 2
    y_end = y_start + size

    crop_img = img[y_start:y_end, x_start:x_end]

    return crop_img

def extract_patch(img, num_patches):
    h, w = img.shape[0], img.shape[1]

    if h != w:
        raise ValueError("Input image must be square.")

    if h % num_patches != 0:
        raise ValueError(f"Image size {h}x{h} cannot be divided into {num_patches}x{num_patches} patches.")

    patch_size = h // num_patches

    shape = (num_patches, num_patches, patch_size, patch_size, 3)
    print(shape)

    strides = [patch_size * s for s in img.strides[:2]] + list(img.strides)
    print(strides)

    patches = stride_tricks.as_strided(img, shape=shape, strides=strides)
    print(patches.shape)

    nrows = patches.shape[0]
    ncols = patches.shape[1]

    fig = plt.figure()
    for row in range(nrows):
        for col in range(ncols):
            idx = row * ncols + col
            ax = fig.add_subplot(nrows, ncols, idx + 1)
            ax.imshow(patches[row, col, :, :])
            ax.tick_params(left=False, bottom=False, labelleft=False, labelbottom=False)

    plt.show()

    return patches

def resize_img(img, factor):
    if factor <= 0:
        raise ValueError("Scale factor must be greater than 0.")

    h, w = img.shape[0], img.shape[1]

    new_h = int(h * factor)
    new_w = int(w * factor)

    resized_img = np.zeros((new_h, new_w, img.shape[2]), dtype=img.dtype)

    for i in range(new_h):
        for j in range(new_w):
            src_i = min(int(i / factor), h - 1)
            src_j = min(int(j / factor), w - 1)
            resized_img[i, j] = img[src_i, src_j]

    plt.show()

    return resized_img

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

def color_jitter(img, hue, saturation, value):
    hue = max(0, min(hue, 360))

    if not (0 <= saturation <= 1) or not (0 <= value <= 1):
        print("Saturation and Value inputs should be within range [0, 1].")
        sys.exit(1)
    
    img_np = np.array(img)

    hsv_img = rgb_to_hsv(img_np)

    hsv_img[np.isnan(hsv_img)] = 0.

    hsv_img[:, :, 0] += random.uniform(0, hue)
    hsv_img[:, :, 1] += random.uniform(0, saturation)
    hsv_img[:, :, 2] += random.uniform(0, value)

    hsv_img[:, :, 0] %= 360.0
    hsv_img[:, :, 1] = np.clip(hsv_img[:, :, 1], 0, 1)
    hsv_img[:, :, 2] = np.clip(hsv_img[:, :, 2], 0, 1)

    jitter_img = hsv_to_rgb(hsv_img)

    return jitter_img

def main():
    img = io.imread("cat.png")

    crop_size = 100
    try:
        cropped_img = random_crop(img, crop_size)
    except ValueError as e:
        print(e)
    else:
        plt.figure(figsize=(10, 5))
        plt.subplot(1, 2, 1)
        plt.title('Original Image')
        plt.imshow(img)
        plt.axis('off')

        plt.subplot(1, 2, 2)
        plt.title('Cropped Image')
        plt.imshow(cropped_img)
        plt.axis('off')

        plt.show()

    num_patches = 4
    patch = extract_patch(img, num_patches)

    factor = 0.2
    try:
        resized_img = resize_img(img, factor)
    except ValueError as e:
        print(e)
    else:
        plt.figure(figsize=(10, 5))
        plt.subplot(1, 2, 1)
        plt.title('Original Image')
        plt.imshow(img)
        plt.axis('off')

        plt.subplot(1, 2, 2)
        plt.title('Resized Image')
        plt.imshow(resized_img)
        plt.axis('off')

    plt.show()

    hue = 180
    saturation = 0.0
    value = 0.0

    try:
        jittered_img = color_jitter(img, hue, saturation, value)
    except ValueError as e:
        print(e)
    else:
        plt.figure(figsize=(15, 10))

        plt.subplot(2, 3, 1)
        plt.title('Original Image')
        plt.imshow(img)
        plt.axis('off')

        for i in range(5):
            jittered_img = color_jitter(img, hue, saturation, value)
            plt.subplot(2, 3, i + 2)
            plt.title(f'Jittered Image {i + 1}')
            plt.imshow(jittered_img)
            plt.axis('off')

        plt.tight_layout()
        plt.show()


if __name__ == "__main__":
    main()