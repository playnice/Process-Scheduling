#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;


struct ProcessRF //RR & FCFS
{
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
    double waitingTime;
    double turnaroundTime;
    bool idleCheck;
    bool inQueue;
};

class Process{
	int name;
    int pid;
    int burstTime;
    int arrivalTime;
    int priority;
    int finishedTime;
    int runningTime;
    int waitTime;
    int turnaroundTime;
public:

    int getName() { return name;  } //SJF

    int getAT() { return arrivalTime; }//SJF

    int getBT() { return burstTime; }//SJF

    int getWT() { return waitTime; }//SJF

    int getTAT() { return turnaroundTime; }//SJF

    void setName(int n) { name = n; }//SJF

    void setAT(int t) { arrivalTime = t; }//SJF

    void setBT(int t) { burstTime = t; }//SJF

    void setWT(int t) { waitTime = t; }//SJF

    void setTAT(int t) { turnaroundTime = t; }//SJF

};

//Created by Wong Zi Xiang
void RR(){
    int i,j,k,n;
    int Quantum;
    int currentTime = 0;
    int totalTime = 0;
    int firstProcess;
    int subtract;
    int processIndex = 1;

    int leftoverQuantum = 0;
    double totalWT = 0;
    double totalTT = 0;
    int totalBT = 0;
    cout << "Enter total number of process(3-10): ";
    cin  >> n;
    while(n < 3 || n > 10){
        if (n < 3){
            cout << "Minimum number of process is 3, please try again: ";
        }
        if (n > 10){
            cout << "Minimum number of process is 3, please try again: ";
        }
        cin >> n;
    }

    ProcessRF p[n];
    ProcessRF temp;

    cout <<"Enter burst time and arrival time for each process:\n";
    for( i=0; i<n; i++) {
        p[i].pid = i;
        cout << "P" << i << "\n";
        cout << "Burst Time: ";
        cin  >> p[i].burstTime;
        cout << "Arrival Time: ";
        cin  >> p[i].arrivalTime;
        if(p[i].arrivalTime == 0){ //If arrival time is 0, schedule will start with idle
            p[i].idleCheck = false;
        }
        if(p[i].arrivalTime > 0){ //If arrival time is not 0, schedule might start with idle
            p[i].idleCheck = true;
        }
        totalTime = totalTime + p[i].burstTime;
        totalBT = totalBT + p[i].burstTime;
        p[i].inQueue = false;
        cout << endl;
    }
    cout << "\nTime Quantum: ";
    cin  >> Quantum;
    int rounds;//Rounds
    //If have remainder, still count
    if(totalTime % Quantum != 0){
        rounds = totalTime/Quantum + 1;
    }
    else{
        rounds = totalTime/Quantum;
    }

    //If there are too many processes finish early, extra round will be needed
    for( i=0; i<n; i++) {
        if(p[i].burstTime > Quantum){
            if(p[i].burstTime % Quantum != 0){
                leftoverQuantum = leftoverQuantum + (p[i].burstTime % Quantum);
            }
        }
    }
    if(leftoverQuantum > Quantum){
    	rounds = rounds + 1;
    }

    //Array spaces allocated according to rounds
    int queueIndex[rounds];
    int execTime[rounds+1];
    int queueBurst[rounds];

    int processSize = (sizeof(p)/sizeof(p[0]));

    //Swapping process based on arrival time and then burst time
    for ( i = 0; i < processSize; i++){
         j = i + 1;
        for (; j <= processSize; j++){
            if(p[j].arrivalTime < p[i].arrivalTime){
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
            if(p[j].arrivalTime == p[i].arrivalTime){
            	if(p[j].burstTime < p[i].burstTime){
            		temp = p[i];
            		p[i] = p[j];
            		p[j] = temp;
            	}
           	}
        }
    }

    queueIndex[0] = p[0].pid;
    queueBurst[0] = p[0].burstTime;
    execTime[0] = 0;
    firstProcess = p[0].arrivalTime;
    totalTime = totalTime + firstProcess;

    int queueCount = 0; //Current process's queue number
    int cutQueue = queueCount;//Is used when there are process needed to be put back into queue

    for(currentTime = firstProcess; currentTime < totalTime; queueCount++){

        //If none of the process arrives at 0, execTime[0] is 0, execTime[1] is set as the start point
       	if(firstProcess != 0){
            execTime[queueCount + 1] = currentTime;
       	}
       	//If there is process arrives at 0, execTime[0] is set as start point
       	if(firstProcess == 0){
            execTime[queueCount] = currentTime;
       	}

       	//If process's burst time is less than quantum time, subtract = current process's burst time
        if(queueBurst[queueCount] < Quantum){
            subtract = queueBurst[queueCount];
        }
        else{
            subtract = Quantum;
        }
        queueBurst[queueCount] = queueBurst[queueCount] - subtract;
        currentTime =  currentTime + subtract;


        //Check if any process arrived during the burst time and add them to queue
        for( j = processIndex; j < processSize; j++){
            if(p[j].arrivalTime <= currentTime && p[j].inQueue == false){
                queueBurst[cutQueue + 1] = p[j].burstTime;
                queueIndex[cutQueue + 1] = p[j].pid;
                p[j].inQueue = true;
                cutQueue++;
            }
            else if(p[j].arrivalTime > currentTime){
               	;
            }
        }
        int currentProcess; //Current process array number

        //To find out current process array number
        for( k = 0; k < n; k++){
            if(p[k].pid == queueIndex[queueCount]){
                currentProcess = k;
            }
        }

        //If current process haven't finish executing, put it back into queue
        if(queueBurst[queueCount] > 0){
            queueBurst[cutQueue + 1] = queueBurst[queueCount];
            queueIndex[cutQueue + 1] = queueIndex[queueCount];
            cutQueue++;
        }
         //Calculate waiting time and turnaround time for the process if it finishes running
        else if(queueBurst[queueCount] == 0){
            p[currentProcess].turnaroundTime = currentTime - p[currentProcess].arrivalTime;
            p[currentProcess].waitingTime = p[currentProcess].turnaroundTime - p[currentProcess].burstTime;
            totalTT += p[currentProcess].turnaroundTime;//Adding to total turnaround time
        }
    }

    //To readjust the array depending on whether there are idling or not
    if(firstProcess != 0){
        execTime[rounds] = currentTime - subtract;
        execTime[rounds + 1] = currentTime;
    }
    else{
        execTime[rounds] = currentTime;
    }

    //calculate total waiting time
    totalWT = totalTT - totalBT;

    //Sort to ascending PID
    for ( i = 0; i < processSize; i++){
         j = i + 1;
        for (; j < processSize; j++){
            if(p[j].pid < p[i].pid){
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    //Table
    cout << "Time Quantum = " << Quantum << endl;
    cout << "+-----+--------------+------------+-----------------+--------------+" << endl
         << "| PID | Arrival Time | Burst Time | Turnaround Time | Waiting Time |" << endl
         << "+-----+--------------+------------+-----------------+--------------+" << endl;

    for( i=0; i<n; i++) {
        printf("| %2d  |      %2d      |     %2d     |       %2.0f        |      %2.0f      |\n"
               , p[i].pid, p[i].arrivalTime, p[i].burstTime, p[i].turnaroundTime, p[i].waitingTime );
        puts("+-----+--------------+------------+-----------------+--------------+");
    }
    double avgTT = totalTT/processSize;
    double avgWT = totalWT/processSize;
    cout << "Average Turnaround Time = " << totalTT << "/" << processSize << " = " << avgTT << "ms" << endl;
    cout << "Average Waiting Time = " << totalWT << "/" << processSize << " = " << avgWT << "ms" << endl;

    cout << "\n\nGantt Chart:" << endl;
    k = 0;
    for(i = 0; i < processSize; i++){
        if(p[i].idleCheck == false){
            break;
        }
        if(p[i].idleCheck == true){
            k++;
        }
    }
    if (k == processSize){
        cout << "+----";
    }
    for( i = 0; i < rounds; i++){
        cout << "+----";
   	}
   	cout << "+" << endl;
   	if (k == processSize){
        cout << "| XX ";
    }
    for( i = 0; i < rounds; i++){
   		cout << "| P" << queueIndex[i] << " ";
   	}
    cout << "|"<< endl;
    if (k == processSize){
        cout << "+----";
    }
    for( i = 0; i < rounds; i++){
        cout << "+----";
   	}
   	cout << "+" << endl;
    if(firstProcess != 0){
        for( i = 0; i <= rounds + 1; i++){
            if(execTime[i] >= 10){
                cout << execTime[i] << "   ";
            }
            else{
                cout << execTime[i] << "    ";
            }
        }
    }
    else{
        for( i = 0; i <= rounds; i++){
            if(execTime[i] >= 10){
                cout << execTime[i] << "   ";
            }
            else{
                cout << execTime[i] << "    ";
            }
        }
    }
}

//Created by Lee Boon Ping
void FCFS(){
    vector <int> sortVec;
    vector <int> tempVec;
    vector <int> displayVec;
    int numOfProcess;
    double tempTurnAroundTime;
    double tempWaitingTime;
    int totalTime = 0;
    double totalTurnAroundTime = 0;
    double totalWaitingTime = 0;
    double AverageTurnAroundTime = 0;
    double AverageWaitingTime = 0;
    int execTime = 0;
    int current;
    int temp;
    int tempArrivedTime;
    bool zero = false;

    cout << " Please enter total number of process (3-10) : ";
    cin >> numOfProcess;
    ProcessRF P[numOfProcess];
    int bTime[10];

    while (numOfProcess < 3 || numOfProcess > 10)
    {
        cout << endl;
        if (numOfProcess < 3)
            cout << " The minimum number of process is 3. Please try again! " << endl;
        else if (numOfProcess > 10)
            cout << " The maximum number of process is 10. Please try again! " << endl;
        cout << " Please enter total number of process (3-10) : ";
        cin >> numOfProcess;
    }

    int firstProcess = 9999;
    for (int i=0; i<numOfProcess; i++)
    {
        cout << endl;
        P[i].pid =i;
        cout << " P" << i << endl;
        cout << " Arrival Time  : ";
        cin >> P[i].arrivalTime;
        if (firstProcess > P[i].arrivalTime)
        {
            firstProcess = P[i].arrivalTime;
        }
        cout << " Burst Time    : ";
        cin >> P[i].burstTime;
        totalTime = totalTime + P[i].burstTime;
        cout << " Priority (1-6): ";
        cin >> P[i].priority;
        while (P[i].priority < 1 || P[i].priority > 6)
        {
            cout << " Priority is only allowed within 1 to 6! " << endl;
            cout << " Priority (1-6): ";
            cin >> P[i].priority;
        }
    }
    for (int i=0; i<numOfProcess; i++)
    {
        if (P[i].arrivalTime == 0)
        {
            zero = true;
        }
    }


    for(int i = 0; i<numOfProcess; i++)
    {
        bTime[i] = P[i].burstTime;
    }

    for(int currentTime=firstProcess; currentTime<totalTime+firstProcess; currentTime++)
    {
        for(int k=0; k<numOfProcess; k++)
        {
            tempArrivedTime = P[k].arrivalTime;
            if(tempArrivedTime == currentTime)
            {
                sortVec.push_back(k);
            }
        }
        for(int x = 0; x<sortVec.size(); x++)
        {
            current = x;
            for(int y = x+1; y<sortVec.size(); y++)
            {
                if(P[sortVec[y]].priority < P[sortVec[current]].priority)
                {
                    current=y;
                }
                else if (P[sortVec[y]].priority == P[sortVec[current]].priority)
                {
                    if(P[sortVec[y]].arrivalTime < P[sortVec[current]].arrivalTime)
                    {
                        current=y;
                    }
                }
            }
           temp=sortVec[x];
           sortVec[x]=sortVec[current];
           sortVec[current]=temp;
        }

        bTime[sortVec[0]] = bTime[sortVec[0]] - 1;
        displayVec.push_back(sortVec[0]);

        if (bTime[sortVec[0]] == 0)
        {
            tempTurnAroundTime = currentTime + 1 - P[sortVec[0]].arrivalTime;
            P[sortVec[0]].turnaroundTime = tempTurnAroundTime;
            tempWaitingTime = P[sortVec[0]].turnaroundTime - P[sortVec[0]].burstTime;
            P[sortVec[0]].waitingTime = tempWaitingTime;
            sortVec.erase(sortVec.begin());
        }
    }


    //Print table
    cout << endl;
    cout << " +-----+--------------+------------+------------+-----------------+--------------+" << endl
         << " | PID | Arrival Time | Burst Time |  Priority  | Turnaround Time | Waiting Time |" << endl
         << " +-----+--------------+------------+------------+-----------------+--------------+" << endl;

    for(int i=0; i<numOfProcess; i++)
    {
        printf(" | %2d  |      %2d      |     %2d     |     %2d     |       %2.0f        |      %2.0f      |\n"
               , P[i].pid, P[i].arrivalTime, P[i].burstTime, P[i].priority, P[i].turnaroundTime, P[i].waitingTime );
        puts(" +-----+--------------+------------+------------+-----------------+--------------+");
    }
    cout << endl;
    for (int i=0; i<numOfProcess; i++)
    {
        totalTurnAroundTime = totalTurnAroundTime + P[i].turnaroundTime;
        totalWaitingTime = totalWaitingTime + P[i].waitingTime;
    }

    AverageTurnAroundTime = (double)totalTurnAroundTime/numOfProcess;
    AverageWaitingTime = (double)totalWaitingTime/numOfProcess;
    cout << " Total Turnaround Time     = " << totalTurnAroundTime << endl;
    cout << " Total Waiting Time        = " << totalWaitingTime << endl << endl;
    cout << " Average Turnaround Time   = " << AverageTurnAroundTime << endl;
    cout << " Average Waiting Time      = " << AverageWaitingTime << endl << endl;



    //Print Gantt Chart
    cout << " ";
    if (zero == false)
    {
       cout << "+------";
    }
    for (int x=0; x<displayVec.size(); x++)
    {

        if (x!=displayVec.size()-1)
        {
            if (displayVec[x] != displayVec[x+1])
            {
                cout << "+------";
            }
        }
        else
            cout << "+------";
    }
    cout << "+" << endl << " ";
    if (zero == false)
    {
        cout << "|  XX" << "  ";
    }
    for (int x=0; x<displayVec.size(); x++)
    {
        if (x!=displayVec.size()-1)
        {
            if (displayVec[x] != displayVec[x+1])
            {
            cout << "|  P" << displayVec[x] << "  ";
            }
        }
        else
            cout << "|  P" << displayVec[x] << "  ";
    }
    cout << "|" << endl << " ";
    if (zero == false)
    {
        cout << "+------";
    }
    for (int x=0; x<displayVec.size(); x++)
    {
        if (x!=displayVec.size()-1)
        {
            if (displayVec[x] != displayVec[x+1])
            {
                cout << "+------";
            }
        }
        else
            cout << "+------";
    }
    cout << "+" << endl << " ";
    if (zero == false)
    {
        cout << "0" << "      ";
        execTime = firstProcess;

    }
    cout << execTime;
    for (int x=0; x<displayVec.size(); x++)
    {
        if (x!=displayVec.size()-1)
        {
            if (displayVec[x] != displayVec[x+1])
            {
                execTime++;
                if (execTime >=10)
                    cout << "     " << execTime;
                else
                    cout << "      " << execTime;
            }
            else if (displayVec[x] == displayVec[x+1])
            {
                execTime++;
            }
        }
        else
        {
            execTime++;
            if (execTime >=10)
                    cout << "     " << execTime;
                else
                    cout << "      " << execTime;
        }
    }
}

//Created by Chow Chan Kit
void SJF(){
	int numOfProcesses;
    do
    {
        cout<<"Enter Total Number of Process(3-10):";
        cin>>numOfProcesses;
    }while(numOfProcesses>10 || numOfProcesses <3);

    Process p[10];

     cout<<"\nEnter Burst Time and Arrival time: \n";
    int in = 0;
    int totalBT = 0;
    for(int i=0;i<numOfProcesses;i++)
    {
        cout<<"\nP["<<i<<"]\n";
        p[i].setName(i);
        cout<<"Arrival Time: ";
        cin>>in;
        p[i].setAT(in);
        cout<<"Burst Time: ";
        cin>>in;
        totalBT += in;
        p[i].setBT(in);
    }
    int at = 0;
    vector<int> readyQueue;
    vector<int> executedSJF;
    int remainingTime[10];

    for(int i = 0; i < numOfProcesses; i++)
    {
        remainingTime[i] = p[i].getBT();        //initialize remaining time = burst time
    }

    for(int time = 0; time < totalBT; time++)
    {
        //store the arrived process into vector(ready queue)
        for(int i=0;i<numOfProcesses;i++)
        {
            at = p[i].getAT();
            if(at == time)
            {
                readyQueue.push_back(i);
            }
        }

        int pos, temp;
        //sort the vector(ready queue)
        for(unsigned int i = 0; i < readyQueue.size(); i++)
        {
            pos = i;
            for(unsigned int j = i+1; j < readyQueue.size(); j++)
            {
                if(remainingTime[readyQueue[j]]<remainingTime[readyQueue[pos]])
                {
                    pos=j;
                }
            }
            temp=readyQueue[i];
            readyQueue[i]=readyQueue[pos];
            readyQueue[pos]=temp;
        }

        remainingTime[readyQueue[0]]--;         //reduce the remaining burst time
        executedSJF.push_back(readyQueue[0]);   //execute the process

        if (remainingTime[readyQueue[0]] == 0)
        {
            int a = time+1 - p[readyQueue[0]].getAT();
            p[readyQueue[0]].setTAT(a);
            int b = p[readyQueue[0]].getTAT() - p[readyQueue[0]].getBT();
            p[readyQueue[0]].setWT(b);
            readyQueue.erase(readyQueue.begin());       //erase the process which finishes execution
        }
    }

    cout << "\nGantt Chart of SJF:" <<endl;
    for(int i = 0; i < executedSJF.size(); i++)
    {
        cout << "| P" << executedSJF[i] << " ";
    }
    cout << "|" <<endl;
    cout << "0" ;
    for (int i=0; i< executedSJF.size();i++)
    {
        if (i >= 10)
        {
            cout << "   "<< i+1;
        }
        else
        {
        cout << "    "<< i+1;
        }
    }

    int totalturnaroundTime = 0;
    int totalwaitingTime = 0;

    cout << "\nProcess Name\t Arrival Time\t Burst Time\t Turnaround Time\tWaiting Time" <<endl;
    for(int i=0;i<numOfProcesses;i++)
    {
        totalturnaroundTime += p[i].getTAT();
        totalwaitingTime += p[i].getWT();
        cout <<"\nP["<<p[i].getName()<<"]\t\t  " <<p[i].getAT()<<"\t\t    "<<p[i].getBT()<<"\t\t    "<<p[i].getTAT()<<"\t\t\t"<<p[i].getWT();
    }

    float avTotalaverageTime = (float)totalturnaroundTime/numOfProcesses;
    float avwaitingTime = (float)totalwaitingTime/numOfProcesses;

    cout << endl;
    cout << "Average Turnaround Time: " << avTotalaverageTime << endl;
    cout << "Average Waiting Time: " << avwaitingTime << endl;

}


void displayChoice(){
		cout << "Please choose the scheduling you want:\n" << endl
			 << "1. FCFS Pre-emptive Priority Scheduling" << endl
			 << "2. Round Robin" << endl
			 << "3. Shortest Job First" << endl
			 << "Q. Quit" << endl << endl
			 << "Choice = ";
	}
int main(){
	char choice;
	displayChoice();

	cin  >> choice;
	while(true){
		if(choice != '1'){
			if(choice != '2'){
				if(choice != '3'){
					if(choice != 'Q'){
						if(choice !='q'){
							cout << "Invalid choice! Please try again!";
							system("cls");
							displayChoice();
							cout << "Choice = ";
							cin  >> choice;
						}
						else{
							break;
						}
					}
					else{
						break;
					}
				}
				else{
					break;
				}
			}
			else{
				break;
			}
		}
		else{
			break;
		}
	}

	while(true){
			switch(toupper(choice)){
			case '1':
					FCFS();
					cout << endl;
					displayChoice();
					cin >> choice;

			case '2':
					RR();
					cout << endl;
					displayChoice();
					cin >> choice;

			case '3':
					SJF();
					cout << endl;
					displayChoice();
					cin >> choice;

			case 'Q':
					return 0;
		}
	}
	return 0;
}
