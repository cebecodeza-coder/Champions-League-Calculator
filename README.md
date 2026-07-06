# ⚽ UEFA Champions League Predictor Calculator

A **C++ console application** developed as part of a **CMPG 121 creative project**.  
The program predicts the probability of a football team advancing through different stages of the **UEFA Champions League (UCL)** using historical performance data and the **Binomial Distribution** model.  

This project combines statistics, probability theory, and CMPG121 C++ programming content to demonstrate how statistical models can be applied to real-world sports analysis.

---

## ✨ Features

- **Group Stage Probability** — Calculates the probability of a team advancing from the group stage (winning 4 or more out of 6 matches).  
- **Knockout Stage Probability** — Predicts the chance of advancing in a two-legged knockout tie (Round of 16 or Quarter-Finals).  
- **Semi-Final Probability** — Estimates the likelihood of reaching the Final using domestic league performance as a statistical proxy.  
- **Final Prediction** — Determines the most likely winner of the Final by comparing average historical domestic win rates.  

---

## 📦 What’s Included

- `main.cpp` — The main program file containing the menu-driven UEFA predictor logic.    
- `leagues.txt` — Text file listing football leagues or league performance data used in the probability model.  
- `TeamNames.txt` — Text file containing the names of all participating teams.  

---

## 🧠 Methodology — Binomial Distribution

The core statistical model behind this project is the **Binomial Distribution**, which models the number of successes (wins) in a fixed number of independent trials (matches).

![Formula](https://latex.codecogs.com/svg.image?\color{white}P(X%3Dk)%20%3D%20\binom{n}{k}%20p^{k}(1-p)^{n-k})

Where:
- **n** = number of matches (e.g., 6 for group stage, 2 for two-legged tie).  
- **p** = probability of winning a single match (based on historical or proxy data).  
- **k** = number of wins.  

**Advancement probabilities:**
- **Group Stage:** ![Formula](https://latex.codecogs.com/svg.image?\bg_black\color{white}P(\text{wins}\ge4)%20=%20\sum_{k=4}^{6}P(X=k))  
- **Knockout Tie:** ![Formula](https://latex.codecogs.com/svg.image?\bg_black\color{white}P(\text{wins}\ge2)) (win both legs or aggregate)

---

## 🛠️ Build & Run (Using Code::Blocks)

### Requirements
- **Code::Blocks IDE** (Version 20.03 or later recommended)  
- **GNU GCC Compiler (MinGW)** — included when you install Code::Blocks with the compiler option.

---

### 🧩 Steps to Set Up and Run

1. **Download the Project**
   - Go to this GitHub repository page.  
   - Click **Code → Download ZIP**.  
   - Extract the ZIP file to a folder (e.g., `Documents/UEFA_Champions_League_Predictor Calculator`).

2. **Open Code::Blocks**
   - Launch Code::Blocks.  
   - Select **File → Open...** and navigate to the extracted folder.  
   - Open **`main.cpp`** (or the `.cbp` project file if included).

3. **Add All Project Files**
   - In the **Project Explorer**, right-click **Sources → Add files...** → select all `.cpp` files (`main.cpp`, `functions.cpp`, etc.).  
   - Make sure **`leagues.txt`** and **`TeamNames.txt`** are in the **same directory** as your source files.

4. **Set Compiler Standard**
   - Go to **Settings → Compiler → Compiler Settings → Compiler Flags**.  
   - Enable ✅ *“Have g++ follow the C++17 ISO C++ language standard (-std=c++17)”*.

5. **Build the Project**
   - Click **Build → Build** or press **Ctrl + F9**.  
   - Wait for the message **“Build succeeded”**.

6. **Run the Program**
   - Click **Build → Run** or press **Ctrl + F10**.  
   - The console window will open, displaying the UCL Predictor menu.

---

### 🧮 Example Output
<img width="600" height="624" alt="image" src="https://github.com/user-attachments/assets/54d293f1-ba04-4599-be83-37dd391609ce" />


---
## ✅ Limitations & Assumptions

-  **Independence of Matches**: Each match is treated as an independent trial; real football matches can have dependencies (e.g., form, injuries, tactics).  
-  **Two-Legged Tie Simplification**: Knockout ties are modeled as two separate “games” with equal weight; aggregate scoring and away goals are not simulated.  
-  **Simplified Probabilities**: Real-world factors such as player availability, team strategies, or external conditions are not included.  
-  **Data Accuracy**: The accuracy of predictions depends entirely on historical win rates provided.  
