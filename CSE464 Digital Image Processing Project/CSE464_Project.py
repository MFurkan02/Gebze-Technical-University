import cv2
import numpy as np

# Initialize the HOG descriptor/person detector
hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())
# The HOGDescriptor is used to detect people in images by analyzing histograms of oriented gradients (HOG). 
# The setSVMDetector method loads the default support vector machine (SVM) for person detection.

def detect_runners(image):
    # Convert the image to grayscale for easier processing (color doesn't matter for detection)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Apply Contrast Limited Adaptive Histogram Equalization (CLAHE) to enhance contrast
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    gray = clahe.apply(gray)

    # Detect people (runners) in the grayscale image using HOG descriptor
    boxes, weights = hog.detectMultiScale(
        gray,
        winStride=(4, 4),        # Step size during scanning
        padding=(8, 8),          # Padding around detected objects
        scale=1.05               # Scale factor for image pyramids
    )

    runner_positions = []
    bounding_boxes = []

    # Iterate through detected bounding boxes
    for (x, y, w, h) in boxes:
        # Calculate the bottom center of each bounding box to mark the runner's position
        bottom_center = (x + w // 2, y + h)
        bounding_boxes.append((x, y, w, h))
        runner_positions.append(bottom_center)

    # Return detected runner positions and their bounding boxes
    return runner_positions, bounding_boxes

def manual_gaussian_blur(image, kernel_size, sigma):
    # Create a Gaussian kernel to apply a Gaussian blur (manual implementation)
    ax = np.linspace(-(kernel_size // 2), kernel_size // 2, kernel_size)
    gauss = np.exp(-0.5 * (ax / sigma) ** 2)
    kernel = np.outer(gauss, gauss)  # Create a 2D Gaussian kernel
    kernel /= kernel.sum()  # Normalize the kernel to avoid changes in brightness

    # Pad the image to handle borders
    padded_image = np.pad(image, kernel_size // 2, mode='reflect')
    rows, cols = image.shape
    blurred_image = np.zeros_like(image)  # Initialize the blurred image

    # Apply the kernel to every pixel in the image (convolution)
    for i in range(rows):
        for j in range(cols):
            region = padded_image[i:i + kernel_size, j:j + kernel_size]  # Get the region around the pixel
            blurred_image[i, j] = np.sum(region * kernel)  # Apply kernel to the region

    # Return the blurred image
    return blurred_image

def manual_sobel(image):
    # Define Sobel kernels for edge detection in x and y directions
    sobel_x = np.array([[-1, 0, 1],
                        [-2, 0, 2],
                        [-1, 0, 1]])
    sobel_y = np.array([[-1, -2, -1],
                        [0,  0,  0],
                        [1,  2,  1]])

    # Pad the image to handle borders
    padded_image = np.pad(image, 1, mode='reflect')
    rows, cols = image.shape
    grad_x = np.zeros_like(image, dtype=np.float32)  # Initialize gradients
    grad_y = np.zeros_like(image, dtype=np.float32)

    # Apply Sobel kernels to compute gradients
    for i in range(rows):
        for j in range(cols):
            region = padded_image[i:i + 3, j:j + 3]  # 3x3 region around the pixel
            grad_x[i, j] = np.sum(region * sobel_x)  # Gradient in x direction
            grad_y[i, j] = np.sum(region * sobel_y)  # Gradient in y direction

    # Compute the magnitude and direction of gradients
    gradient_magnitude = np.sqrt(grad_x**2 + grad_y**2)
    gradient_direction = np.arctan2(grad_y, grad_x) * 180 / np.pi
    gradient_direction = (gradient_direction + 180) % 180  # Normalize direction to [0, 180)

    # Return the gradient magnitude and direction
    return gradient_magnitude, gradient_direction

def manual_canny(image, low_threshold, high_threshold):
    # Apply Gaussian blur to the image before edge detection
    blurred = manual_gaussian_blur(image, kernel_size=5, sigma=1.4)

    # Compute gradient magnitude and direction
    gradient_magnitude, gradient_direction = manual_sobel(blurred)

    # Initialize an empty image for non-maximum suppression
    rows, cols = gradient_magnitude.shape
    nms = np.zeros((rows, cols), dtype=np.float32)

    # Perform non-maximum suppression
    for i in range(1, rows - 1):
        for j in range(1, cols - 1):
            direction = gradient_direction[i, j]
            magnitude = gradient_magnitude[i, j]

            # Compare gradients in the direction of the edge
            if (0 <= direction < 22.5) or (157.5 <= direction <= 180):
                neighbors = [gradient_magnitude[i, j - 1], gradient_magnitude[i, j + 1]]
            elif 22.5 <= direction < 67.5:
                neighbors = [gradient_magnitude[i - 1, j + 1], gradient_magnitude[i + 1, j - 1]]
            elif 67.5 <= direction < 112.5:
                neighbors = [gradient_magnitude[i - 1, j], gradient_magnitude[i + 1, j]]
            else:
                neighbors = [gradient_magnitude[i - 1, j - 1], gradient_magnitude[i + 1, j + 1]]

            # Keep the current pixel only if it's a local maximum
            if magnitude >= max(neighbors):
                nms[i, j] = magnitude

    # Apply double thresholding to classify strong and weak edges
    strong_edges = (nms >= high_threshold).astype(np.uint8)
    weak_edges = ((nms >= low_threshold) & (nms < high_threshold)).astype(np.uint8)

    # Connect weak edges to strong edges using edge tracking by hysteresis
    edges = np.zeros_like(nms, dtype=np.uint8)
    strong_i, strong_j = np.where(strong_edges)
    edges[strong_i, strong_j] = 255

    weak_i, weak_j = np.where(weak_edges)
    for i, j in zip(weak_i, weak_j):
        if np.any(edges[i - 1:i + 2, j - 1:j + 2] == 255):
            edges[i, j] = 255

    # Return the final edge-detected image
    return edges

def manual_hough_transform(edges, rho_res=1, theta_res=np.pi / 180):
    # Calculate the dimensions of the accumulator array based on the image size
    height, width = edges.shape
    diag_len = int(np.sqrt(height**2 + width**2))  # Max distance from origin
    accumulator = np.zeros((2 * diag_len, int(np.pi / theta_res)), dtype=int)

    # Perform Hough Transform to detect lines
    for y in range(height):
        for x in range(width):
            if edges[y, x] == 255:  # Only consider edge pixels
                for theta_idx in range(accumulator.shape[1]):
                    theta = theta_idx * theta_res
                    rho = int(x * np.cos(theta) + y * np.sin(theta))
                    rho_idx = rho + diag_len  # Shift the rho value to avoid negative indices
                    accumulator[rho_idx, theta_idx] += 1

    # Return the accumulator
    return accumulator

def detect_lanes(image):
    # Convert the image to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Perform edge detection using Canny
    edges = manual_canny(gray, low_threshold=50, high_threshold=150)

    # Perform Hough Transform to detect lines
    accumulator = manual_hough_transform(edges)

    # Set a threshold for detecting strong lines in the accumulator
    threshold = np.max(accumulator) * 0.15
    lines = []

    # Identify the lines from the accumulator
    for rho_idx in range(accumulator.shape[0]):
        for theta_idx in range(accumulator.shape[1]):
            if accumulator[rho_idx, theta_idx] > threshold:
                rho = rho_idx - accumulator.shape[0] // 2
                theta = theta_idx * np.pi / accumulator.shape[1]
                x1 = int(rho * np.cos(theta) - 1000 * np.sin(theta))
                y1 = int(rho * np.sin(theta) + 1000 * np.cos(theta))
                x2 = int(rho * np.cos(theta) + 1000 * np.sin(theta))
                y2 = int(rho * np.sin(theta) - 1000 * np.cos(theta))
                lines.append((x1, y1, x2, y2))

    # Classify detected lines as horizontal or vertical based on their slopes
    horizontal_lanes = [line for line in lines if abs(line[1] - line[3]) < 20]
    vertical_lanes = [line for line in lines if abs(line[0] - line[2]) < 20]
    
    # Return detected horizontal and vertical lanes
    return horizontal_lanes, vertical_lanes

def draw_lanes(image, horizontal_lanes, vertical_lanes):
    # Draw horizontal lanes (green lines)
    for lane in horizontal_lanes:
        cv2.line(image, (0, lane[1]), (image.shape[1], lane[1]), (0, 255, 0), 2)

def draw_finish_line(image, vertical_lanes):
    # If there are no vertical lanes detected, print an error and return 0
    if not vertical_lanes:
        print("No vertical lanes detected!")
        return 0

    # Only draw the red finish line for the rightmost vertical lane
    finish_line = vertical_lanes[-1]
    x1, x2 = finish_line[0], finish_line[2]
    height = image.shape[0]
    cv2.line(image, (x1, 0), (x2, height), (0, 0, 255), 2)  # Red finish line
    return (x1 + x2) // 2  # Return the center of the finish line

def draw_runners(image, bounding_boxes, runner_positions):
    # Draw bounding boxes around the runners
    for (x, y, w, h) in bounding_boxes:
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)
    
    # Draw the runner's positions as green dots
    for (x, y) in runner_positions:
        cv2.circle(image, (x, y), 5, (0, 255, 0), -1)

def rank_runners_after_drawing(image, bounding_boxes):
    # Rank runners based on their horizontal position (right to left)
    runners_with_centers = [(x + w // 2, y + h, (x, y, w, h)) for (x, y, w, h) in bounding_boxes]
    ranked_runners = sorted(runners_with_centers, key=lambda x: x[0], reverse=True)
    
    # Return the ranked bounding boxes
    return [runner[2] for runner in ranked_runners]

def main(image_path, output_path):
    # Load the input image
    image = cv2.imread(image_path)
    if image is None:
        print("Failed to load image. Check the file path.")
        return

    # Detect lanes and runners in the image
    horizontal_lanes, vertical_lanes = detect_lanes(image)
    runner_positions, bounding_boxes = detect_runners(image)

    # Draw lanes, runners, and finish line on the image
    draw_lanes(image, horizontal_lanes, vertical_lanes)

    # Draw the finish line and get its position
    finish_line_center = draw_finish_line(image, vertical_lanes)

    if finish_line_center:
        print(f"Finish line center position: {finish_line_center}")

    draw_runners(image, bounding_boxes, runner_positions)

    # Rank the runners based on their positions
    ranked_runners = rank_runners_after_drawing(image, bounding_boxes)
    print("Ranked Runners (from right to left):")
    for idx, runner_position in enumerate(ranked_runners):
        print(f"{idx + 1}: Position: {runner_position}")

    # Draw the rank of each runner above their bounding box
    for idx, (x, y, w, h) in enumerate(ranked_runners):
        rank_text = f"{idx + 1}"
        cv2.putText(image, rank_text, (x + w // 2, y - 10), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.9, (255, 255, 0), 2)

    # Add a watermark text at the bottom of the image
    cv2.putText(image, "Pac-Men Group Project", (10, image.shape[0] - 10), 
                cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

    # Save the output image
    cv2.imwrite(output_path, image)


# Example usage
image_path = 'photofinish.jpg'  # Path to the input image
output_path = 'output.jpg'  # Path to save the output image
main(image_path, output_path)