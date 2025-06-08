#pragma once
class EntityBase {
public:
    virtual ~EntityBase() = default;    
    virtual void update(float deltatime) = 0;
    virtual void draw() const = 0;
};
