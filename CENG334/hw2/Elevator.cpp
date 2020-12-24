#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#include <string>
#include <fstream>
#include <sstream>

#include <unistd.h>

#include "monitor.h"

using namespace std;


bool sim_started = false;


enum ElevatorState
{
    IDLE,
    MOVING_UP,
    MOVING_DOWN
};

typedef struct
{
    int num_floors      ,
        num_people      ,
        weight_capacity ,
        person_capacity ,
        travel_time     ,
        idle_time       ,
        in_out_time     ;
} ElevatorInfo;

typedef struct
{
    int weight  ,
        from    ,
        to      ,
        priority,
        id      ;
    ElevatorState direction;
} PersonInfo;


class ElevatorMonitor : public Monitor
{
private:
    void PrintPerson(PersonInfo info)
    {
        string priority = (info.priority == 2) ? "lp" : "hp";
        
        cout << "Person (" << info.id << ", "
             << priority << ", " << info.from
             << " -> " << info.to << ", "
             << info.weight << ")";
    }

    void PrintElevator()
    {
        int n = (int)dest_q.size();
        string direction = (state == IDLE) ? "Idle"
            : ((state == MOVING_UP) ? "Moving-up" : "Moving-down");

        cout << "Elevator (" << direction << ", "
             << current_weight << ", " << current_count
             << ", " << current_floor << " ->";
        
        if(n > 0)
            cout << " ";

        for(int i = 0; i < n; i++)
            if(i == n-1) cout << dest_q[i];
            else         cout << dest_q[i] << ",";
        cout << ")" << endl;
    }

    void InsertDestination(int dest)
    {
        if (dest == current_floor)
            return;

        if(find(dest_q.begin(), dest_q.end(), dest) == dest_q.end())
        {
            dest_q.emplace_back(dest);
            
            if(state == MOVING_UP)
                sort(dest_q.begin(), dest_q.end());
            else if(state == MOVING_DOWN)
                sort(dest_q.rbegin(), dest_q.rend());
        }
    }

    void ChangeStateAndNotifyRequest(ElevatorState newState)
    {
        state = newState;
        
        request.notifyAll();
    }

    void Request(PersonInfo& info)
    {
        while(true)
        {
            bool behind = true;
            if(
                (state == MOVING_DOWN && info.from < current_floor) ||
                (state == MOVING_UP   && info.from > current_floor)
            ){
                behind = false;
            }

            if(state == IDLE || (info.direction == state && !behind))
                break;
            else
                request.wait();
        }

        if(state == IDLE)
        {
            if(info.from < current_floor)
                ChangeStateAndNotifyRequest(MOVING_DOWN);
            else if(info.from > current_floor)
                ChangeStateAndNotifyRequest(MOVING_UP);
        }
            
        InsertDestination(info.from);

        PrintPerson(info); cout << " made a request" << endl;
        PrintElevator();
    }

    bool Enter(PersonInfo& info)
    {
        while(current_floor != info.from)
        {
            if(info.priority == 1)
                hp_enter.wait();
            else
                lp_enter.wait();

            if(current_floor == info.from)
            {
                if(state == IDLE)
                    return false;
            }
        }

        if(state != IDLE && state != info.direction)
            return false;
            
        bool weight =
            current_weight + info.weight <= elevatorInfo.weight_capacity;
        bool count = current_count < elevatorInfo.person_capacity;

        if(!count || !weight)
            return false;

        current_count++;
        current_weight += info.weight;

        if(state == IDLE)
        {
            if(info.to < current_floor)
                ChangeStateAndNotifyRequest(MOVING_DOWN);
            else
                ChangeStateAndNotifyRequest(MOVING_UP);
        }
            
        InsertDestination(info.to);

        PrintPerson(info); cout << " entered the elevator" << endl;
        PrintElevator();

        return true;
    }

public:
    Condition pillow, request, leave, hp_enter, lp_enter;
    vector<int> dest_q;
    ElevatorInfo elevatorInfo;
    ElevatorState state;
    int current_floor;
    int current_weight;
    int current_count;
    int served;

    ElevatorMonitor(ElevatorInfo info)
        : pillow(this), request(this), leave(this), state(IDLE),
            hp_enter(this), lp_enter(this), elevatorInfo(info)
    {
        current_floor = 0;
        current_weight = 0;
        current_count = 0;
        served = 0;
    }

    int RequestAndEnter(PersonInfo info)
    {
        __synchronized__;

        Request(info);
        return Enter(info);
    }

    void Leave(PersonInfo info)
    {
        __synchronized__;

        while(current_floor != info.to)
            leave.wait();

        current_count--;
        current_weight -= info.weight;

        PrintPerson(info); cout << " has left the elevator" << endl;
        PrintElevator();

        served++;
    }

    void IdleSleep()
    {
        __synchronized__;

        if(!sim_started)
            sim_started = true;

        request.notifyAll();
        pillow.WaitTime(elevatorInfo.idle_time * 1000);
    }

    void Serve()
    {
        __synchronized__;

        int dest = dest_q.front();
        while(current_floor != dest)
        {
            pillow.WaitTime(elevatorInfo.travel_time * 1000);
            dest = dest_q.front();
            
            current_floor += (state == MOVING_UP) ? 1 :-1;

            if(current_floor == dest)
            {
                dest_q.erase(dest_q.begin());

                if(dest_q.size() == 0)
                {
                    ChangeStateAndNotifyRequest(IDLE);
                }
            }

            PrintElevator();
        }
        
        request.notifyAll();
        leave.notifyAll();
        pillow.WaitTime(elevatorInfo.in_out_time * 500);

        request.notifyAll();
        hp_enter.notifyAll();
        pillow.WaitTime(elevatorInfo.in_out_time * 250);

        request.notifyAll();
        lp_enter.notifyAll();
        pillow.WaitTime(elevatorInfo.in_out_time * 250);
    }
};

struct PersonParameter
{
    ElevatorMonitor *monitor;
    PersonInfo *info;
};

void* ControllerThread(void *varg)
{
    ElevatorMonitor *monitor = (ElevatorMonitor*)varg;

    while(monitor->served < monitor->elevatorInfo.num_people)
    {
        if(monitor->state == IDLE)
        {
            monitor->IdleSleep();
        }
        else
        {
            monitor->Serve();
        }
    }

    return NULL;
}

void* PersonThread(void *varg)
{
    ElevatorMonitor *monitor = ((PersonParameter*)varg)->monitor;
    PersonInfo *info = ((PersonParameter*)varg)->info;
    delete ((PersonParameter *)varg);

    info->direction = (info->to < info->from) ? MOVING_DOWN : MOVING_UP;

    //while(!sim_started); // Wait until controller starts.

    while(!monitor->RequestAndEnter(*info));
    monitor->Leave(*info);

    return NULL;
}

ElevatorInfo ReadElevatorInfo(ifstream& input_file);
PersonInfo ReadPersonInfo(ifstream& input_file, int pid);


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Only filename!\n");
        exit(0);
    }
    ifstream input_file(argv[1]);

    /* CONTROLLER */
    ElevatorInfo info = ReadElevatorInfo(input_file);
    ElevatorMonitor monitor(info);

    pthread_t controller;
    pthread_create(&controller, NULL, ControllerThread, (void *)&monitor);

    /* Create Persons */
    int personCount = info.num_people;
    vector<pthread_t> person(info.num_people, 0);
    vector<PersonInfo> personInfo(info.num_people);
    for(int i = 0; i < info.num_people; i++)
    {
        personInfo[i] = ReadPersonInfo(input_file, i);
        PersonParameter* pp = new PersonParameter({&monitor, &personInfo[i]});
        pthread_create(&person[i], NULL, PersonThread, (void *)pp);
    }

    input_file.close();
    
    /* Join Persons */
    for(int i = 0; i < info.num_people; i++)
    {
        pthread_join(person[i], NULL);
    }
    pthread_join(controller, NULL);


    return 0;
}


ElevatorInfo ReadElevatorInfo(ifstream& input_file)
{
    string inputs;
    getline(input_file, inputs);

    ElevatorInfo info;
    
    istringstream iss(inputs);
    for(int i = 0; i < 7; i++)
        iss >> ((int *)&info)[i];

    return info;
}

PersonInfo ReadPersonInfo(ifstream& input_file, int pid)
{
    string inputs;
    getline(input_file, inputs);

    PersonInfo info;
    
    istringstream iss(inputs);
    for(int i = 0; i < 4; i++)
        iss >> ((int *)&info)[i];
    ((int *)&info)[4] = pid;

    return info;
}
