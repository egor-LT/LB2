
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;


struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int startTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};


vector<Process> generateProcesses(int n) {
    vector<Process> processes;
    srand(time(0));
    for (int i = 0; i < n; i++) {
        Process p;
        p.id = i + 1;
        p.arrivalTime = rand() % 10; 
        p.burstTime = rand() % 10 + 1; 
        p.priority = rand() % 5 + 1; 
        p.remainingTime = p.burstTime;
        processes.push_back(p);
    }
    return processes;
}


void roundRobin(vector<Process> processes, int timeQuantum) {
    cout << "\nRound Robin Scheduling:\n";
    int time = 0;
    queue<Process*> readyQueue;
    vector<Process*> remaining;

    for (auto& p : processes) {
        remaining.push_back(&p);
    }

    while (!remaining.empty()) {
        for (auto it = remaining.begin(); it != remaining.end();) {
            Process* p = *it;
            if (p->arrivalTime <= time) {
                readyQueue.push(p);
                it = remaining.erase(it);
            } else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            Process* current = readyQueue.front();
            readyQueue.pop();
            int execTime = min(current->remainingTime, timeQuantum);
            time += execTime;
            current->remainingTime -= execTime;
            if (current->remainingTime > 0) {
                readyQueue.push(current);
            } else {
                current->completionTime = time;
                current->turnaroundTime = current->completionTime - current->arrivalTime;
                current->waitingTime = current->turnaroundTime - current->burstTime;
                cout << "Process " << current->id << " completed at time " << time << "\n";
            }
        } else {
            time++;
        }
    }
}


void fcfs(vector<Process> processes) {
    cout << "\nFCFS Scheduling:\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0;
    for (auto& p : processes) {
        if (time < p.arrivalTime) {
            time = p.arrivalTime;
        }
        p.startTime = time;
        p.completionTime = time + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        time = p.completionTime;

        cout << "Process " << p.id << " started at " << p.startTime
             << " and completed at " << p.completionTime << "\n";
    }
}


void priorityScheduling(vector<Process> processes) {
    cout << "\nPriority Scheduling:\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.priority < b.priority;
    });

    int time = 0;
    for (auto& p : processes) {
        if (time < p.arrivalTime) {
            time = p.arrivalTime;
        }
        p.startTime = time;
        p.completionTime = time + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        time = p.completionTime;

        cout << "Process " << p.id << " with priority " << p.priority
             << " completed at time " << p.completionTime << "\n";
    }
}


void analyzePerformance(vector<Process> processes) {
    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;
    for (auto& p : processes) {
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
    }
    cout << "\nAverage Waiting Time: " << totalWaitingTime / processes.size() << "\n";
    cout << "Average Turnaround Time: " << totalTurnaroundTime / processes.size() << "\n";
}


int main() {
    int n = 5; 
    int timeQuantum = 3; 

    vector<Process> processes = generateProcesses(n);

    cout << "Generated Processes:\n";
    for (auto& p : processes) {
        cout << "Process ID: " << p.id
             << ", Arrival Time: " << p.arrivalTime
             << ", Burst Time: " << p.burstTime
             << ", Priority: " << p.priority << "\n";
    }

  
    roundRobin(processes, timeQuantum);
    analyzePerformance(processes);

    
    fcfs(processes);
    analyzePerformance(processes);

    
    priorityScheduling(processes);
    analyzePerformance(processes);

    return 0;
}
