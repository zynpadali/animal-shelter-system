#include <iostream>
#include <vector>
using namespace std;
//class Animal
class Animal {
private:
    string name;
    string species;
    string animalID;
    bool adopted;

    static int totalAnimals; //return total animal objects

public:
    //Constructor: initializes variables
    Animal(string n, string s, string ID) {

    name = n;
    species = s;
    animalID = ID;
    adopted = false; //default
    totalAnimals++; // increase total count when new animal is created
    }
//Display animal informaiton
    void displayInfo() const {
        cout << "Name: " << name << ", Species: " << species  << ", ID: "<< animalID
        << ", Adopted: " << (adopted  ? "Yes":"No") << endl;
    }

    //Getter methods
    string getName() const {
        return name;
    }

    string getID() const {
        return animalID;
    }

    string getSpecies() const {
        return species;
    }

    //Set adoption status(adopted or not)
    void setAdoptionStatus(bool status){
        adopted = status;
    }

    //return current adoption status
    bool isAdopted() const {
        return adopted;
    }

    //return total animal object number
    static int getTotalAnimals() {
        return totalAnimals;
    }
};

//initialize static variable
int Animal::totalAnimals = 0;


//Abstract Class
class ShelterUser {

protected:
    string name;
    int userID;

public:
    //constructor:initializes variables
    ShelterUser(string n, int id) {
        name = n;
        userID = id;
    }

    //Getter methods
    string getName() const {
        return name;
    }

    int getID() const {
        return userID;
    }

    //pure virtual function
    virtual void displayInfo() const =0;

};


//Adopter class

class Adopter : public ShelterUser {
private:
   vector<Animal*> animals;

public:
    //constructor
    Adopter(string n, int id): ShelterUser(n, id) {}


    void adoptPet(Animal* a) {
        if (a->isAdopted() == false) {
            a->setAdoptionStatus(true);
            animals.push_back(a);
            cout << "Adopter " << name << " is adopting " << a->getName() << endl;
        }else {
            cout << a->getName() << "is already adopted" << endl;
        }
    }

    void returnPet(Animal* a) {

        bool found = false;

        for (size_t i = 0; i < animals.size(); i++) {
            if (a == animals[i]) {
                animals.erase(animals.begin() + i);
                a->setAdoptionStatus(false);
                found = true;
                cout << "Adopter " << name << " returned " << a->getName() << "." << endl;
                break;
            }
        }
        if (found == false) {
            cout << name << "not found" << endl;
        }
    }

    //Display adopter info
    void displayInfo() const override {
        cout << "Adopter Name: " << name << ", ID: " << userID << ", Adopted Pets: " << animals.size() << endl;
    }
};


//ShelterStaff class
class ShelterStaff : public ShelterUser {
private:
    vector<Animal*> shelterA;
public:
    //Constructor
    ShelterStaff(string n, int id) : ShelterUser(n, id) {}

    void addAnimal(Animal* a) {

        shelterA.push_back(a);
        cout << "Adding animal: " << a->getName() <<
            "(" << a->getSpecies() << "), ID:"<< a->getID()<< endl;
        cout << "Total animals: " << Animal :: getTotalAnimals() << endl;
    }


    void removeAnimal(string id) {
        bool found = false;
        for (size_t i = 0; i < shelterA.size(); i++) {
            if (shelterA[i]->getID() == id) {
                shelterA.erase(shelterA.begin() + i);
                found = true;
                cout << "Removed animal: "<< id << endl;
                break;
            }
        }
        if (found == false) {
            cout << name << "not found" << endl;
        }
    }
    void displayInfo() const override {
        cout << "Staff Name: " << name << ", ID: " << userID << ", Animals in Shelter: " << shelterA.size() << endl;
    }
};

//ShelterSystem class

class ShelterSystem {
private:
    vector<Animal*> animals;// all animals
    vector<ShelterUser*> users; //All users

public:

    void addUser(ShelterUser* user) {
        users.push_back(user); //store user
        if (dynamic_cast<Adopter*>(user))
            cout << "Adding new adopter: " << user->getName() << " (ID: " << user->getID() << ")" << endl;
        else if (dynamic_cast<ShelterStaff*>(user))
            cout << "Adding new staff: " << user->getName() << " (ID: " << user->getID() << ")" << endl;
    }


    Animal* searchAnimal(const string& name, size_t i = 0) {

        if (i >= animals.size()) {
            return nullptr;
        }
         if (name == animals[i]->getName()) {
             return animals[i];
         }
        return searchAnimal(name, i + 1);
    }


    void displayAllAnimals() const {

        cout << "Displaying all animals:" << endl;
        for (size_t i = 0; i < animals.size(); i++) {
            animals[i]->displayInfo();
        }

        cout << "Total animals: " << Animal::getTotalAnimals() << endl;
    }

    // add animal to main system list
    void addAnimalToSystem(Animal* a) {
        animals.push_back(a);
    }
    void displayAllUsers() const {
        for (size_t i = 0; i < users.size(); i++) {
            users[i]->displayInfo();
        }
    }
};

 //Main function
int main() {

    cout << "Animal Shelter System Initialized." << endl;
    ShelterSystem shelter;

    //create users
    Adopter* Adil =new Adopter("Adil Koçari", 101);
    shelter.addUser(Adil);

    Adopter* zeynep =new Adopter("Zeynep Adalı", 101);
    shelter.addUser(zeynep);

    ShelterStaff* Eleni = new ShelterStaff("Eleni Doe", 102);
    shelter.addUser(Eleni);

    cout<< endl;

    //create animals

    Animal* buddy = new Animal("Buddy", "Sheep", "D101");
    Eleni->addAnimal(buddy);
    shelter.addAnimalToSystem(buddy);

    Animal* daisy = new Animal("Daisy", "Dog", "D104");
    Eleni->addAnimal(daisy);
    shelter.addAnimalToSystem(daisy);

    Animal* whiskers = new Animal("Whiskers", "Cat", "C102");
    Eleni->addAnimal(whiskers);
    shelter.addAnimalToSystem(whiskers);

    cout << endl;


    Adil->adoptPet(daisy);
    Adil->adoptPet(buddy);
    zeynep->adoptPet(whiskers);
    Adil->returnPet(buddy);

    cout << endl;

    shelter.displayAllUsers();
    cout << endl;

    shelter.displayAllAnimals();
    cout << endl;


   Animal* found = shelter.searchAnimal("Whiskers");

    if (found != nullptr) {
        cout << "Animal found: " << found->getName() << endl;
    }else {
        cout<< "Animal not found" << endl;
    }



    return 0;
};
