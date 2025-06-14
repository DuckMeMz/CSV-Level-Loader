#include "Force.h"


Vector2D Force::GenerateFrictionForce(const Vector2D velocity, const double K)
{
    Vector2D frictionForce = Vector2D(0.0, 0.0);

    //calculate the friction direction i.e. the inverse of the unit vector
    Vector2D frictionDirection = velocity.unitVector() * -1.0;

    //calculate the friction magnitude, for now we'll just use the constant K
    double frictionMagnitude = K;

    //Calculate the final friction force
    frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}

Vector2D Force::GenerateDragForce(const Vector2D velocity, const double K)
{
    Vector2D dragForce = Vector2D(0.0, 0.0);
    
    if (velocity.magnitudeSquared() > 0.0)
    {
        //calculate the drag direction i.e. the inverse of the unit vector
        Vector2D dragDirection = velocity.unitVector() * -1.0;

        //calculate the drag magnitude, k * |v|^2
        double dragMagnitude = K * velocity.magnitudeSquared();

        //Generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

Vector2D Force::GenerateWeightForce(double mass)
{
    Vector2D weight = Vector2D(0.0, -(mass * 9.8 * PIXELS_PER_METER));
    return weight;
}

Vector2D Force::GenerateGravitationalForce(const GameObject& a, const GameObject& b, double G)
{
    Vector2D distance = b.GetPosition() - a.GetPosition();
    double distanceSquared = distance.magnitudeSquared();

    Vector2D attractionDirection = distance.unitVector();
    double attractionMagnitude = G * (a.GetMass() * b.GetMass()) / distanceSquared;

    Vector2D attractionForce = attractionDirection * attractionMagnitude;

    return attractionForce;
}

Vector2D Force::GenerateSpringForce(const Vector2D position, const Vector2D anchorPosition, const double restLength, const double K)
{
    //Calculate the distance between the anchor and the object
    Vector2D distance = position - anchorPosition;

    //Find the spring displacement considering the rest length
    double displacement = distance.magnitude() - restLength;

    //Calculate the direction and magnitude of the spring force
    Vector2D springDirection = distance.unitVector();
    double springMagnitude = -K * displacement;

    //Calculate the final resulting spring force vector
    Vector2D springForce = springDirection * springMagnitude;

    return springForce;
}
