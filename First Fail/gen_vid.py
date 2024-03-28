import os
import imageio

# Directory containing all the board directories
videos_dir = "Videos"

# Function to create a webm video from images in a directory
def create_video(board_size):
    board_dir = os.path.join(videos_dir, f"Board_{board_size}")
    images = []
    for filename in sorted(os.listdir(board_dir)):
        if filename.endswith(".png"):  # Assuming images are in PNG format
            images.append(imageio.imread(os.path.join(board_dir, filename)))
        print('on filename', filename)
    output_file = os.path.join(videos_dir, f"Board_{board_size}.mp4")
    imageio.mimsave(output_file, images, fps=25)  # Adjust fps as needed

# Iterate through each board directory
"""
for board_size in range(4, 75):
    create_video(board_size)
"""
create_video(40)
