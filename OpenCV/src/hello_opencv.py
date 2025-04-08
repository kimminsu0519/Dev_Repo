import cv2

capture = cv2.VideoCapture(0)
capture.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

while True:
    ret, frame = capture.read()
    src = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    src_canny = cv2.Canny(src, 50, 100)
    cv2.imshow("VideoFrame", frame)
    cv2.imshow("Canny", src_canny)
    
    if cv2.waitKey(10) == 27:
        break

capture.release()
cv2.destroyAllWindows()