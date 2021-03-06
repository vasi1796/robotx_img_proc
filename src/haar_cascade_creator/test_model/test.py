import cv2

# this is the cascade we just made. Call what you want
cube_cascade = cv2.CascadeClassifier(
    'D:/dev/Programming/Git/robotx_img_proc/src/haar_cascade_creator/opencv-haar-classifier-training/classifier/cascade.xml')

cap = cv2.VideoCapture(1)

while 1:
    ret, img = cap.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # add this
    # image, reject levels level weights.
    cubes = cube_cascade.detectMultiScale(gray)

    # add this
    for (x, y, w, h) in cubes:
        cv2.rectangle(img, (x, y), (x + w, y + h), (255, 255, 0), 2)
    #cv2.rectangle(img, (0, 0), (200, 200), (255, 255, 0), 2)
    cv2.imshow('img', img)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()
