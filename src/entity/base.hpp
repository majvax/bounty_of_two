
class EntityBase {
    public:
    EntityBase();
    virtual void process(float deltatime) = 0;
    virtual void draw() const = 0;
};