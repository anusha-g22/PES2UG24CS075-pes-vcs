# PES-VCS (Mini Git Implementation)

## 📌 Overview
This project implements a simplified version control system similar to Git.  
It supports content-addressable storage, staging, commits, and history tracking.

---

## ⚙️ Features

- `pes init` → Initialize repository
- `pes add <file>` → Stage files
- `pes status` → Show file status
- `pes commit -m "msg"` → Create commit
- `pes log` → Show commit history

---

## 🏗️ Architecture

- **Blob** → Stores file contents
- **Tree** → Represents directories
- **Commit** → Snapshot of project

Structure:
