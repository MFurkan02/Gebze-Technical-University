# 🏃‍♂️ Athlete Photo-Finish & Lane/Line Detection 

[![Python](https://img.shields.io/badge/Python-3.8%2B-blue.svg)](https://www.python.org/)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.x-green.svg)](https://opencv.org/)
[![NumPy](https://img.shields.io/badge/NumPy-1.20%2B-choices.svg)](https://numpy.org/)

This project is a **Computer Vision** application developed to analyze photo-finish moments in track and field races. It automatically detects running lanes and the finish line on the track, localizes the athletes (runners), and ranks them dynamically based on their proximity to the finish line.

---

## 🎓 Academic Context

This system was developed as a term project for the **CSE464 Digital Image Processing** course. The primary academic objective was to master the underlying mathematical principles of computer vision.

> ⚠️ **Core Constraint:** To demonstrate a deep theoretical and practical understanding of digital image processing, all foundational feature extraction, filtering, and edge detection techniques were **implemented entirely from scratch using raw matrix operations (NumPy)**. High-level image processing library functions (such as `cv2.Canny`, `cv2.GaussianBlur`, or `cv2.HoughLines`) were strictly avoided for the core algorithmic steps.

---

## 🚀 Key Features

* **HOG + SVM Based Human Detection:** Integration of Histogram of Oriented Gradients (HOG) descriptor and Linear SVM classifier to accurately detect runners in the frame.
* **From-Scratch Mathematical Implementation:** Manual coding of mathematical models for `Gaussian Blur`, `Sobel Gradient`, `Non-Maximum Suppression (NMS)`, `Hysteresis Thresholding (Canny)`, and `Hough Transform`.
* **Dynamic Contrast Enhancement:** Adaptation of CLAHE (Contrast Limited Adaptive Histogram Equalization) to optimize object visibility under varying lighting conditions.
* **Lane & Finish Line Isolation:** Classification of detected lines in Hough space into horizontal (lanes) and vertical (finish line) components based on their slopes.
* **Automated Ranking & Visualization:** Coordinate-based dynamic ranking relative to the finish line (rightmost vertical line) and overlaying results on the output image.

---

## 🛠 Image Processing Pipeline & Algorithms

The system processes the input race image through the following sequential stages:

### 1. Preprocessing & Contrast Enhancement
The input image is converted to grayscale, and the **CLAHE** algorithm is applied to enhance local contrast, ensuring optimal feature extraction.

### 2. Manual Gaussian Blur (Image Smoothing)
To mitigate high-frequency noise, a 2D Gaussian kernel is mathematically constructed and convolved over the image using reflection padding (`reflect mode`) to elegantly handle boundaries.

$$G(x, y) = \frac{1}{2\pi\sigma^2} e^{-\frac{x^2 + y^2}{2\sigma^2}}$$

### 3. Manual Canny Edge Detection
* **Sobel Filter:** Computes the image gradients ($G_x, G_y$) in both horizontal and vertical directions to evaluate edge magnitudes.
* **Non-Maximum Suppression (NMS):** Suppresses non-peak pixels along the gradient direction to thin out thick edges into single-pixel lines.
* **Hysteresis Double Thresholding:** Classifies weak and strong edges using lower and upper thresholds, discarding weak edges that are not connected to a strong edge.

### 4. Manual Hough Line Transform
Edge pixels are mapped into the polar coordinate system ($\rho = x \cos\theta + y \sin\theta$) to vote in an accumulator matrix. Local maxima exceeding a specific threshold are isolated as detected lines.

### 5. Object Detection & Proximity Ranking
`cv2.HOGDescriptor` extracts bounding boxes and bottom-center coordinates for each runner. The rightmost vertical line is designated as the official finish line, and runners are indexed and ranked from right to left based on their X-axis coordinates.
