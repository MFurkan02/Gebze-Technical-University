# 🎓 Gebze Technical University - Computer Engineering Portfolio

Welcome to my official repository archiving coursework, laboratory assignments, and milestone design projects completed throughout my **Computer Engineering** B.Sc. education at **Gebze Technical University (GTU)**. 

This repository serves as a comprehensive professional portfolio demonstrating my software development capabilities, algorithmic problem-solving depth, system-level design methodologies, and implementation autonomy across various computational paradigms.

---

## 📋 Curriculum & Course Layout

The repository is structured semantically based on the GTU Department of Computer Engineering syllabus. Below is an overview of the courses, technical scopes, and language stacks utilized:

### 🔹 Core Programming & Paradigms

#### CSE102 - Computer Programming
* **Language/Stack:** C / GCC
* **Focus:** Procedural programming fundamentals, explicit heap memory management, pointer arithmetic, custom structures, file I/O streams, and algorithmic thinking with rigorous edge-case testing.

#### CSE241 - Object-Oriented Programming
* **Language/Stack:** C++ (Modern C++11/14/17 standards) / G++
* **Focus:** Object-oriented design patterns, strict encapsulation, polymorphic architectures, inheritance hierarchies, complex operator overloading, custom templates (Generic Programming), RAII, and exception handling.

#### CSE341 - Programming Languages
* **Language/Stack:** Common Lisp (SBCL), Prolog (SWI-Prolog)
* **Focus:** Functional and Logic programming paradigms. Deep-dive implementations including custom lexical interpreters, complex recursive data processing, tail-call optimization, and rule-based declarative inference systems.

---

### 🔹 Data Structures & System Engineering

#### CSE222 - Data Structures and Algorithms
* **Language/Stack:** Java / JDK
* **Focus:** Abstract Data Types (ADTs). Custom boilerplate-free implementation and Big-O ($O$) asymptotic complexity analysis of Linear Data Structures, Trees (Binary Search, AVL, Red-Black, B-Trees), Priority Queues/Heaps, Graph Matrices/Adjacency lists (BFS, DFS, Dijkstra, A*), and advanced sorting/searching configurations.

#### CSE344 - System Programming
* **Language/Stack:** C / POSIX Linux API
* **Focus:** Low-level Unix/Linux system boundaries. Multiprocessing control (`fork`, `exec`, `wait`), Inter-Process Communication (IPC) via anonymous pipes, named FIFOs, message queues, and shared memory. Advanced non-blocking I/O multiplexing, atomic signal handling, and multi-process architectural patterns.

#### CSE312 - Operating Systems
* **Language/Stack:** C, C++ / POSIX pthreads
* **Focus:** Internal mechanics of modern OS kernels. CPU Scheduling simulation, Virtual Memory orchestration (Paging, Segmentation, TLB caching), Deadlock avoidance models (Banker's Algorithm), and thread-level concurrency synchronization using Semaphores, Mutexes, and Condition Variables.

---

### 🔹 Advanced Domains & Intelligent Systems

#### CSE461 - Computer Graphics
* **Language/Stack:** C++ / Modern OpenGL, GLSL
* **Focus:** The programmable graphics pipeline, 3D affine mathematical transformations (Homogeneous coordinates), custom camera matrix look-at loops, Vertex and Fragment Shaders, Phong/Blinn-Phong local illumination models, and real-time polygon rendering.

#### CSE464 - Digital Image Processing
* **Language/Stack:** Python / NumPy (Core Engines), OpenCV (I/O, Analytics visualization)
* **Focus:** Mathematical feature extraction and matrix manipulation without high-level library dependencies. Implementations include spatial domain processing, Local Contrast Tuning via CLAHE, **from-scratch 2D Gaussian Filtering (with reflection boundary padding)**, **custom Canny Edge Detection** (incorporating Sobel Operators, Non-Maximum Suppression, and Hysteresis Double Thresholding), and **Polar Hough Line Transformations**. Applied to automated athletic photo-finish tracking, lane boundary isolation, and athlete ranking.

#### CSE454 - Data Mining
* **Language/Stack:** Python / NumPy, Pandas, Multiprocessing, Matplotlib, Mlxtend (Benchmark Only)
* **Focus:** Scalable pattern mining and transactional business intelligence. Built completely **from-scratch BFS (Apriori) and DFS tree-structured (FP-Growth)** pattern mining engines using raw data pointers, hash-maps, and custom data-chunk workers to bypass Python's Global Interpreter Lock (GIL). Designed mathematical rules engines evaluating **Support, Confidence, Lift, and Leverage**, paired with low-level runtime heap profiling (`tracemalloc`) and a statistical filter to systematically isolate and reject spurious coincidental correlations. Includes a transactional reverse-logistics sub-system tracking co-returned "toxic" product bundles.

>>>>>>> c09d398 (docs: add and structure deployment READMEs for root, DIP, and Data Mining modules)

