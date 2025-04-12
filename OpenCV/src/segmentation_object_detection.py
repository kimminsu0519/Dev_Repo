import sys
import os
import numpy as np
import cv2

# 현재 파일 기준 base 디렉토리 설정
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(BASE_DIR, '../data')

# 숫자 템플릿 이미지 10개 로딩
def load_digits():
    img_digits = []
    for i in range(10):
        filename = os.path.join(DATA_DIR, 'digits/digit{}.bmp'.format(i))
        img = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
        if img is None:
            return None
        img_digits.append(img)
    return img_digits

# 입력된 숫자 이미지와 가장 유사한 템플릿 인덱스 반환
def find_digit(img, img_digits):
    max_idx = -1
    max_ccoeff = -1
    img = cv2.resize(img, (100, 150))  # 크기 맞추기
    for i in range(10):
        res = cv2.matchTemplate(img, img_digits[i], cv2.TM_CCOEFF_NORMED)
        if res[0, 0] > max_ccoeff:
            max_idx = i
            max_ccoeff = res[0, 0]
    return max_idx

# 메인 함수
def main():
    # 입력 영상 경로
    image_path = os.path.join(DATA_DIR, 'digits_print.bmp')

    # 입력 영상 불러오기
    src = cv2.imread(image_path)
    if src is None:
        print('Image load failed!')
        return

    # 숫자 템플릿 이미지 로딩
    img_digits = load_digits()
    if img_digits is None:
        print('Digit image load failed!')
        return

    # 입력 영상 이진화 및 레이블링
    src_gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
    _, src_bin = cv2.threshold(src_gray, 0, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)
    cnt, _, stats, _ = cv2.connectedComponentsWithStats(src_bin)

    # 결과 영상 초기화
    dst = src.copy()

    for i in range(1, cnt):
        x, y, w, h, s = stats[i]
        if s < 1000:
            continue

        # 각 숫자 영역 잘라서 인식
        digit_img = src_gray[y:y+h, x:x+w]
        digit = find_digit(digit_img, img_digits)

        # 결과 출력
        cv2.rectangle(dst, (x, y, x+w, y+h), (0, 255, 0), 2)
        cv2.putText(dst, str(digit), (x, y - 4), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)

    cv2.imshow('dst', dst)
    cv2.waitKey()
    cv2.destroyAllWindows()

# main 실행
if __name__ == '__main__':
    main()