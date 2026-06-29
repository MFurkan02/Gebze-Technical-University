# 🛒 Scalable E-Commerce Pattern Mining & Churn Analysis

[![Python](https://img.shields.io/badge/Python-3.8%2B-blue.svg)](https://www.python.org/)
[![Pandas](https://img.shields.io/badge/Pandas-Matplotlib-orange.svg)](https://pandas.pydata.org/)
[![Data-Mining](https://img.shields.io/badge/Field-Data%20Mining%20%7C%20Analytics-green.svg)]()

An enterprise-grade transactional analytics platform designed to uncover cross-selling paths, purchase dependencies, and item-level churn risks across large e-commerce databases. This repository contains end-to-end data mining engines implementing both breadth-first search (BFS) and tree-based depth-first search (DFS) mining paradigms.

---

## 🎓 Academic Context & Core Constraint

This project was developed as a comprehensive final design project for the **CSE454 Data Mining** course.

> ⚠️ **The "From-Scratch" Constraint:** To prove total algorithmic and mathematical comprehension of transactional pattern matching, **all machine learning and data mining libraries (such as `mlxtend` or `scikit-learn`) were strictly prohibited for the production pipeline**. All tree nodes, rule generation metrics, and database scanning routines were developed entirely from scratch using raw matrix, multiprocessing pools, and hash-map structures. High-level libraries were utilized **solely** within a dedicated benchmarking suite to explicitly validate the performance parity of our custom models.

---

## 🚀 Key Architectural Modules

The platform is divided into five highly specialized infrastructure components:

### 1. `apriori.py` (Parallelized Combinatorial BFS Engine)
* **Downward-Closure Optimization:** Leverages the anti-monotone Apriori property to trim itemset explosions before physical scans.
* **Stress Test Sensitivity Engine:** Features automated telemetry routines to map execution scaling across custom support boundaries (`min_support = [500, 400, 300]`).

### 2. `fptree.py` (Tree-Structured Compressed DFS Engine)
* **Prefix-Tree Construction:** Builds memory-efficient `FPNode` graph structures with sequential pointers, compressing transactions without losing semantic association links.
* **Conditional Database Mining:** Evaluates path structures recursively by reversing conditional pattern bases rather than generating thousands of candidate permutations.

### 3. `churnanalysis.py` (Transactional Return & Churn Risk Engine)
* **Risk Evaluation Matrix:** Segregates valid commercial conversions from reverse-logistics returns (tracking the custom business string prefix `C`).
* **Toxic Bundle Profiling:** Applies an aggressive low-threshold custom scanning model to pinpoint items that are co-returned together, identifying structural manufacturing defects or misleading catalog indexing.

### 4. `utils.py` (Performance Telemetry & Mathematical Utility Suite)
* **Asynchronous Multiprocessing Pool:** Implements a map-reduce style chunk divider (`parallel_scan`) that splits the transaction database among physical CPU cores, bypassing Python's Global Interpreter Lock (GIL) via worker processes.
* **Low-Level Memory Profiler:** Utilizes `tracemalloc` hooks to gauge peak RAM footprint byte allocations during structural mutations.
* **Statistically Rigorous Rule Mining:** Computes **Support Count, Confidence, Lift, and Leverage** indices from scratch to construct item recommendation ranks.
* **Spurious Pattern Detection Filter:** Screens out coincidental pairs (where $0.8 \le \text{Lift} \le 1.2$). This isolates items that appear together purely due to high individual popularity rather than true affinity.

### 5. `compare_all.py` (Empirical Benchmarking Framework)
* **Production Validation:** Performs true empirical testing against industry-standard libraries (`mlxtend.apriori` & `mlxtend.fpgrowth`).
* **Resource Auditing:** Couples telemetry with custom memory profiling to render side-by-side performance plots comparing execution time and RAM footprint.

---

## 🛠 Complete Processing Pipeline

The transactional engine runs through a clear, multi-tier execution lifecycle:

```text
[Raw Retail CSV] ──► [Noise & Cancel Parsing] ──► [Data Splits: Sales vs. Returns]
                                                            │
         ┌──────────────────────────────────────────────────┴─────────────────────────────────────────────────┐
         ▼ (Pattern Exploration)                                                                              ▼ (Churn Diagnostics)
[Custom Apriori Engine]  OR  [Custom FP-Growth Engine]                                              [Product Return Analysis]
         │                                   │                                                                │
         ├───────────────────────────────────┤                                                                ▼
   (Multiprocessing Scans)           (DFS Graph Compression)                                         (Toxic Bundle Detection)
         │                                   │                                                                │
         └─────────────────┬─────────────────┘                                                                │
                           ▼                                                                                  ▼
              [Advanced Rule Extraction]                                                              [Risk Metrics Output]
            (Metrics: Lift, Conf, Leverage)
                           │
                           ▼
             [Spurious Pattern Filtering] ──► [Advanced Recommendation System Output]