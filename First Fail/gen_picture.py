import os
import numpy as np
from PIL import Image, ImageDraw
import subprocess
import shutil

def draw_board(img, dim):
    print(img, dim)

    
def load_board(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Determine board size
    n = lines[0].count('.') // 2

    boards = []
    current_board = []

    for line in lines:
        line = line.strip()

        
        if line:
            current_board.append(line.split())
        else:
            if current_board:
                boards.append(np.array(current_board))
                current_board = []


    return boards
def generate_chessboard_image(n, board):
    board_size = 1200
    square_size = board_size/n  # Adjust the size of each square as needed
    line_width = int(square_size/4)
    Queen_color = 'Red'
    img = Image.new('RGB', (board_size, board_size), color='white')
    draw = ImageDraw.Draw(img)

    # Iterate through each square
    for i in range(n):
        for j in range(n):
            x0 = i * square_size
            y0 = j * square_size
            x1 = x0 + square_size
            y1 = y0 + square_size

            # Draw black and white squares
            if (i + j) % 2 == 0:
                draw.rectangle([x0, y0, x1, y1], fill='white')
            else:
                draw.rectangle([x0, y0, x1, y1], fill='black')
            if board[i][j] == '♕':
                # Draw the Queen character
                draw.ellipse([x0, y0, x1, y1], fill=Queen_color)
       
                
    return img

def pretty_print_boards(b):
    for a in b:
        for c in a:
            for d in c:
                print(d, end=' ')
            print()
        print('\n\n')
        
        
for b in range(31, 74):
    
    board = 'Boards/{0}.txt'.format(b)
    boards = load_board(board)
    try:
        os.mkdir('Videos/Board_{0}'.format(b))
    except:
        print('already exists')
    for fndx, frame in enumerate(boards):
        image = generate_chessboard_image(len(frame), frame)
        image.save('Videos/Board_{0}/Frame_{1:06}.png'.format(b, fndx))
    #os.system('ffmpeg -framerate 10 -pattern_type glob -i \'*.png\' Videos/board_{0}.webm'.format(b))
    
