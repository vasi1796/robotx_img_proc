import cv2
import os

cap = cv2.VideoCapture(0)
pic_num = 228

if __name__ == "__main__":
    if not os.path.exists('real'):
        os.makedirs('real')

    while True:
        ret, mat = cap.read()
        if mat is not None:
            mat = cv2.cvtColor(mat, cv2.COLOR_BGR2GRAY)
            mat = cv2.resize(mat, (130, 100))
            cv2.imshow("cube", mat)
            key = cv2.waitKey(1)
            if key == ord('v'):
                cv2.imwrite("real/img" + str(pic_num) + ".jpg", mat)
                pic_num += 1
