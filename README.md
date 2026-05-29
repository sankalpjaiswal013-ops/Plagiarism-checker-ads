# 🛡️ Advanced Plagiarism Detection Engine

A high-performance, browser-based plagiarism detection system that demonstrates the real-world application of Advanced Data Structures and Algorithms. The entire indexing and similarity detection pipeline runs client-side, eliminating the need for a backend server while maintaining fast and scalable document analysis.

## 🚀 Live Demo

LINK- https://plagiarism-checker-ads.vercel.app/


## 📌 Project Overview

This project analyzes large collections of documents and detects textual similarities by combining efficient string-matching techniques with custom-built indexing structures. Every core data structure was implemented from scratch in JavaScript to showcase algorithmic design, optimization, and memory-efficient searching.

## 🧠 Algorithms & Data Structures

### 🔹 Rabin-Karp Rolling Hash

* Documents are divided into 4-word k-grams.
* Each k-gram is converted into a rolling hash using:

  * Base = 31
  * Modulus = 1,000,000,009
* Enables efficient O(n + m) pattern matching and similarity detection.

### 🔹 B-Tree (Order 3)

* Serves as the primary document indexing structure.
* Supports efficient O(log n) insertion and search operations.
* Optimized for handling large datasets while minimizing memory overhead.

### 🔹 Extendible Hash Table

* Stores k-gram hash values for rapid lookup.
* Dynamically expands through bucket splitting and directory doubling.
* Maintains near-constant-time access while efficiently managing collisions.

### 🔹 Min-Heap Priority Queue

* Maintains the highest-scoring matching documents.
* Efficiently retrieves the Top-K plagiarism candidates.
* Ensures fast ranking of similarity results.

## ⚡ Key Features

* Fully client-side plagiarism detection
* No backend or database required
* Fast document indexing and retrieval
* Dynamic collision handling using extendible hashing
* Efficient ranking of similarity matches
* Interactive and responsive user interface
* Demonstrates practical implementation of Advanced Data Structures

## 💻 Tech Stack

### Frontend

* React (Babel Standalone)
* HTML5
* CSS3
* JavaScript (ES6+)

### UI & Styling

* Glassmorphism Design
* Tabler Icons
* Inter Font

### Deployment

* Vercel

## ⚙️ Running Locally

1. Clone the repository:

   ```bash
   git clone <repository-url>
   ```

2. Navigate to the project folder.

3. Open `index.html` in any modern web browser.

4. Paste text into the plagiarism checker and explore the indexing and matching algorithms in action.

## 🎯 Learning Objectives

This project was developed to bridge theoretical concepts from Advanced Data Structures with practical software engineering. It showcases how hashing, balanced trees, priority queues, and efficient string-matching algorithms can be combined to build a real-world text similarity detection system entirely in the browser.

## 📄 License

This project is intended for educational and portfolio purposes.
