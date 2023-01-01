import numpy as np
import cv2
from PIL import Image


def imread_gray(path):
    """
    Reads an image in gray. Image type is transformed from uint8 to float, and
    range of values is reduced from [0, 255] to [0, 1].
    """
    I = Image.open(path).convert('L')  # PIL image opening and converting to gray.
    I = np.asarray(I)  # Converting to Numpy array.
    I = I.astype(np.float64) / 255
    return I

def calculate_gaussian(sigma):
    x = np.arange(math.floor(-3 * sigma), math.ceil(3*sigma + 1))

    gauss = (1/(math.sqrt(2*math.pi) * sigma)) * np.power(math.e,
        - np.power(x, 2) / (2 * math.pow(sigma, 2))
    )
    sum = np.sum(gauss)
    return (gauss / sum).reshape(1, -1)

def gaussdx(sigma):
    x = np.arange(math.floor(-3 * sigma), math.ceil(3*sigma + 1))

    gaussdx = (-1/(math.sqrt(2*math.pi) * math.pow(sigma, 3))) * x * np.power(math.e,
        - np.power(x, 2) / (2 * math.pow(sigma, 2))
    )
    sum = np.sum(np.abs(gaussdx))
    return np.flip(gaussdx / sum).reshape(1, -1)

def convolution(image, kernel):
    return cv2.filter2D(image, -1, kernel)

def convolution_2k(image, kernel1, kernel2):
    return convolution(convolution(image, kernel1), kernel2)


def imagedx(image, sigma):
    return convolution_2k(image,
        calculate_gaussian(sigma).T,
        gaussdx(sigma)
    )

def gradient_magnitude(image, sigma):
    I_x = imagedx(image, sigma)
    I_y = imagedy(image, sigma)

    return np.arctan2(I_y, I_x), np.sqrt(
        np.power(I_x, 2) + np.power(I_y, 2)
    )

def nonmax(gradient, magnitude):
    gradient = np.where(gradient > 0, gradient, gradient + math.pi)
    gradient /= math.pi / 8
    gradient = np.floor(gradient).astype(np.uint8)
    gradient[gradient == 8] = 0

    n_tab = [
        [(1, 0), (-1, 0)],
        [(-1, -1), (1, 1)],
        [(-1, -1), (1, 1)],
        [(0, 1), (0, -1)],
        [(0, 1), (0, -1)],
        [(-1, 1), (1, -1)],
        [(-1, 1), (1, -1)],
        [(1, 0), (-1, 0)]
    ]

    im = np.ones_like(magnitude, dtype=np.uint8)
    for y in range(sy := magnitude.shape[0]):
        for x in range(sx := magnitude.shape[1]):
            n_offsets = n_tab[gradient[y, x]]
            neigbours = [(x+ox, y+oy) for ox, oy in n_offsets]

            for nx, ny in neigbours:
                if 0 <= nx < sx and 0 <= ny < sy and \
                magnitude[y, x] <= magnitude[ny, nx]:
                    im[y, x] = 0
                    break

    return im

def findedges_nonmax(image, sigma, theta):
    I_grad, I_mag = gradient_magnitude(image, sigma)
    I_mag *= I_mag > theta

    return nonmax(I_grad, I_mag)


# Canny edge detector
def hysteresis(image, sigma, high, low):
    edges_high = findedges_nonmax(image, sigma, high)
    edges_low = findedges_nonmax(image, sigma, low)

    _, mask_low, _, _ = cv2.connectedComponentsWithStats(edges_low)

    edges = np.zeros_like(image, dtype=np.uint8)

    for y in range(image.shape[0]):
        for x in range(image.shape[1]):
            if edges_high[y, x]:
                edge_mask = mask_low == mask_low[y, x]
                edges[edge_mask] = 1
                edges_high[edge_mask] = 0

    return edges


if __name__ == '__main__':
    museum = imread_gray('images/museum.jpg')
    thr = 0.16
    high = thr
    low = 0.04

    canny = hysteresis(museum, 1, high, low)
