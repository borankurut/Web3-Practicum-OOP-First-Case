#include <iostream>
#include <iomanip> // for leading 0's
#include <vector>

using namespace std;

void error(string er){
    throw runtime_error(er);
}

class Date{
private:
    int month;
    int day;
    int year;
public:
    Date(int newMonth, int newDay, int newYear){
        setMonth(newMonth); setDay(newDay); setYear(newYear);
    }

    // getters setters:
    void setMonth(int newMonth){
        if(newMonth <= 0 || newMonth > 12)
            error("Month cannot be " + newMonth + '.');
        month = newMonth;
    }

    void setDay(int newDay){
        if(newDay <= 0 || newDay > 31)
            error("Day of the month cannot be " + newDay + '.');
        day = newDay;
    }

    void setYear(int newYear){
        if(newYear < 0)
            error("Year cannot be " + newYear + '.');
        year = newYear;
    }

    int getMonth(){return month;}
    int getDay(){return day;}
    int getYear(){return year;}

    friend ostream& operator<<(ostream& os, const Date& dt);
};

class Time{
private:
    int hour;   // hours
    int min;    // minutes
    int sec;    // seconds;
public:
    Time(int newHour, int newMin, int newSecond){
        setHour(newHour); setMin(newMin); setSec(newSecond);
    }

    // getters setters
    int getHour(){return hour;}
    int getMin(){return min;}
    int getSec(){return sec;}

    int setHour(int newHour){
        if(newHour < 0 || newHour >= 24)
            error("Hour cannot be " + newHour + '.');
        hour = newHour;
    }
    int setMin(int newMin){
        if(newMin < 0 || newMin >= 60)
            error("Minute cannot be " + newMin + '.');
        min = newMin;
    }
    int setSec(int newSec){
        if(newSec < 0 || newSec >= 60)
            error("Second cannot be " + newSec + '.');
        sec = newSec;
    }

    friend ostream& operator<<(ostream& os, const Time& t);
};

class DateAndTime{
public:
    Date dt;
    Time t;
public:
    DateAndTime(Date newDate, Time newTime): dt(newDate), t(newTime){/*empty*/}

    static DateAndTime currentDate(){return DateAndTime(Date(9, 5, 2022), Time(16, 3, 50)/*trivial date*/);}

    friend ostream& operator<<(ostream& os, const DateAndTime& dtt);
};


class Vehicle{
private:
    string hgsNum;              // HGS number of the vehicle
    string ownerName;           // Owner name of the vehicle
    vector<DateAndTime> dates;  // Dates vehicle crossed the bridge
    double balance;             // Balance
    
public:
    Vehicle(string newHgsNum, string newOwnerName): 
        hgsNum(newHgsNum), ownerName(newOwnerName), balance(0){/*empty*/}

    virtual string type() const = 0;  //returns the type of the vehicle

    // getters setters
    string getHgs(){return hgsNum;}
    string getOwnerName(){return ownerName;}

    void setHgs(string newHgs){hgsNum = newHgs;}
    void setOwnerName(string newOwnerName){ownerName = newOwnerName;}

    void addDate(DateAndTime dtt){dates.push_back(dtt);}
    void cost(double c){
        balance -= c;
        this->addDate(DateAndTime::currentDate());
    } 

    friend ostream& operator<<(ostream& os, const Vehicle& v);
};

class Car: public Vehicle{
private:

public:
    Car(string newHgsNum, string newOwnerName): Vehicle(newHgsNum, newOwnerName){/*empty*/}
    string type() const override {return "Car";}
};

class Minibus: public Vehicle{
private:

public:
    Minibus(string newHgsNum, string newOwnerName): Vehicle(newHgsNum, newOwnerName){/*empty*/}
    string type() const override {return "Minibus";}
};

class Bus: public Vehicle{
private:

public:
    Bus(string newHgsNum, string newOwnerName): Vehicle(newHgsNum, newOwnerName){/*empty*/}
    string type() const override {return "Bus";}
};

class Booth{
public:
    const double CostCar = 8.25;
    const double CostMinibus = 10.75;
    const double CostBus = 23.25;

    double takings = 0;              // revenue of the booth
    vector<Vehicle*> passedVehicles;

    void payment(Vehicle& vehicle){
        double c;                       // cost of current vehicle
        if(vehicle.type() == "Car")
            c = CostCar;
        else if(vehicle.type() == "Minibus")
            c = CostMinibus;
        else if(vehicle.type() == "Bus")
            c = CostBus;
        vehicle.cost(c);
        takings += c;
        passedVehicles.push_back(&vehicle);
    }

    friend ostream& operator<<(ostream& os, const Booth& b);   
};

class Manager{
public:
    vector<Booth*> booths;
    double sumOfTakings(){
        double sum = 0.0;
        for(const Booth* b : booths)
            sum += b->takings;
        return sum;
    }
};

// << overrides
ostream& operator<<(ostream& os, const Date& dt){
    os << setw(2) << setfill('0') << dt.month;
    os << '/';
    os << setw(2) << setfill('0') << dt.day;
    os << '/' << dt.year;
    return os;
}

ostream& operator<<(ostream& os, const Time& t){
    os << setw(2) << setfill('0') << t.hour;
    os << ':';
    os << setw(2) << setfill('0') << t.min;
    os << ':';
    os << setw(2) << setfill('0') << t.sec;
    return os;
}

ostream& operator<<(ostream& os, const Vehicle& v){
    os << "Type: " << v.type()<< "   " << "HGS: " << v.hgsNum 
        << "   " << "Owner Name: " << v.ownerName<< "   Balance: " << v.balance << endl;
    return os;
}

ostream& operator<<(ostream& os, const DateAndTime& dtt){
    os << dtt.t << " - " << dtt.dt;
    return os;
}

ostream& operator<<(ostream& os, const Booth& b){
    os << "Booth takings: " << b.takings << endl;
    os << "Passed vehicles: ";
    for(const Vehicle* v : b.passedVehicles)
        cout << *v;
    return os;
}

int main(){
    Manager manager;
    Booth b1;
    Booth b2;

    Car car("1234567890", "Bugra Arslan");
    Minibus minibus("2345678901", "Ichika Nito");
    Bus bus("3456789012", "Rick Sanchez");

    manager.booths.push_back(&b1);
    manager.booths.push_back(&b2);
    

    cout << "Before passing bridge:\n";

    cout << "booth1: " << b1 << endl;
    cout << "booth2: " << b2 << endl;
    cout << "sum of takings: ";
    cout << manager.sumOfTakings() << endl << endl;

    cout << car << " is passing from booth1\n";
    b1.payment(car);
    
    cout << "booth1: " << b1 << endl;
    cout << "booth2: " << b2 << endl;
    cout << "sum of takings: ";
    cout << manager.sumOfTakings() << endl << endl;

    cout << minibus << "and" << bus << " are passing from booth2\n";
    b2.payment(minibus);
    b2.payment(bus);


    cout << "booth1: " << b1 << endl;
    cout << "booth2: " << b2 << endl;
    cout << "sum of takings: ";
    cout << manager.sumOfTakings() << endl << endl;
}