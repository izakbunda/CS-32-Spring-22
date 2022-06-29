
class Landmark {

  public:
    Landmark( string name ) : m_name( name ) {}
    virtual ~Landmark() {};
    
    virtual string name() const {
        return m_name;
    }
    virtual string color() const { // the only landmark that uses a different color is the hospital hence it has its own color() function
        return "yellow";
    }
    virtual string icon() const = 0; // this is a purely virtual function because all landmarks have different icons
    
  private:
    string m_name;  // all landmarks share this trait - having a name
    
};

class Hotel : public Landmark {
  public:
    Hotel( string name ) : Landmark( name ) {}
    virtual ~Hotel() { // destructor
        cout << "Destroying the hotel " << name() << "." << endl;
    }
    virtual string icon() const {
        return "bed";
    }
  private:
    
    
};

class Restaurant : public Landmark {
  public:
    Restaurant( string name, int capacity ) : Landmark( name ), m_capacity( capacity) {}
    virtual ~Restaurant() {
        cout << "Destroying the restaurant " << name() << "." << endl;
    }
    virtual string icon() const {
        if (m_capacity < 40) { // if the capacity is below 40, then it should display a small knife/fork
            return "small knife/fork";
        }
        else { // if the capacity is at 40 and above, then it should display a large knife/fork
            return "large knife/fork";
        }
    }
    
  private:
    int m_capacity; // this is unique to restaurant
};

class Hospital : public Landmark {
  public:
    Hospital ( string name ) : Landmark( name ) {}
    virtual ~Hospital() {
        cout << "Destroying the hospital " << name() << "." << endl;
    }
    virtual string icon() const {
        return "H";
    }
    virtual string color() const {
        return "blue";
    }
  private:
    
};
