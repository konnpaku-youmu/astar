import cv2
import numpy as np

map_width = 768
map_height = 768


class Node():
    def __init__(self, pos, free=True):
        self.position = pos
        self.f_score = np.Inf
        self.g_score = np.Inf
        self.parent = None
        self.neighbours = []
        self.visited = False
        self.isFree = free

    def update(self, node):
        self.f_score = node.f_score + 1
        pass

    def dist(self, node):
        return np.sqrt((self.position[0] - node.position[0]) ** 2 +
                       (self.position[1] - node.position[1]) ** 2)


def construct_map(map_img):
    node_list = []
    for row in range(map_img.shape[0]):
        for col in range(map_img.shape[1]):
            if map_img[row, col, 0] == 0:
                new_node = Node((row, col))
                node_list.append(new_node)

    return node_list


def main():
    map = cv2.imread("./map_view.bmp")
    start_pt = (460, 300)
    end_pt = (425, 280)

    cv2.circle(map, start_pt, 2, (0, 0, 255), 2)
    cv2.circle(map, end_pt, 2, (255, 0, 0), 2)
    cv2.imshow("map", map)
    cv2.waitKey(0)


if __name__ == "__main__":
    main()
