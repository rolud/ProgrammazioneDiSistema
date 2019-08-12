//
// Created by rolud on 12/08/2019.
//

#ifndef ESERCITAZIONE4_JOB_H
#define ESERCITAZIONE4_JOB_H


class Job {
public:
    Job (int id, int start_time, int duration);

    int getId() const;
    void setId(int id);
    int getDuration() const;
    void setDuration(int duration);
    int getExecution_time() const;
    void setExecution_time(int execution_time);
    int getStart_time() const;
    void setStart_time(int start_time);
    int getWait_time() const;
    void setWait_time(int wait_time);
    int getCompletation_time() const;
    void setCompletation_time(int completation_time);

    bool operator > (const Job& j) const;
    bool operator < (const Job& j) const;

private:
    int id;                // identificativo univoco
    int duration;          // durata del compito da svolgere in ms
    int execution_time;    // quanto tempo è già stato dedicato all'attività
    int start_time;        // istante in cui far partire l'attività
    int wait_time;         // tempo totale di attesa (somma di tutte le attese)
    int completation_time; // istante di tempo in cui il gestore ha terminato l'attività

};


#endif //ESERCITAZIONE4_JOB_H
