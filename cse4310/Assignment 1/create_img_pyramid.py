import numpy as np
import os
import skimage.io as io
from skimage.transform import resize

def create_img_pyramid(img, pyramid_height, output_filename):
    h, w = img.shape[0], img.shape[1]
    
    for i in range(1, pyramid_height):
        scale_factor = 2 ** i
        new_h = h // scale_factor
        new_w = w // scale_factor
        
        image_resized = resize(img, (new_h, new_w), anti_aliasing=True)
        
        if img.dtype == np.uint8:
            image_resized = (image_resized * 255).astype(np.uint8)
        
        name, ext = os.path.splitext(output_filename)
        resized_filename = f"{name}_{scale_factor}x{ext}"
        
        io.imsave(resized_filename, image_resized)
        print(f"Saved {resized_filename}")
    print("All new images are saved to current directory")

if __name__ == "__main__":
    input_file = "cat.jpg"
    
    name, ext = os.path.splitext(input_file)
    
    img = io.imread(input_file)
    
    img_np = np.array(img)
    
    create_img_pyramid(img_np, pyramid_height=4, output_filename=f"{name}{ext}")