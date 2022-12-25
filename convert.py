from sys import argv
import cv2 as cv

# img = open(argv[1], "r")
# print(img.read())

img = cv.imread(argv[1])
#img = cv.cvtColor(img, cv.COLOR_BGR2RGB)
cv.imwrite("Scene.jpg",img)
cv.imshow("ppm output", img)
cv.waitKey(0)
