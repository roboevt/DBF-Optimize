#include <iostream>
#include <vector>

struct Plane {
    double payload, emptyWeight, wingArea, CoL;
    virtual double minPayload() = 0;
    virtual double maxPayload() = 0;
    virtual double minEmptyWeight() = 0;
    virtual double maxEmptyWeight() = 0;
    virtual double minWingArea() = 0;
    virtual double maxWingArea() = 0;
    virtual double minCoL() = 0;
    virtual double maxCoL() = 0;
    virtual double calculateScore() = 0;
private:
    virtual double maxCruise() = 0;
    virtual double maxThrust() = 0;
};

struct M2Plane: public Plane {
    double minPayload() override {
        return emptyWeight * 0.3;
    }
    double maxPayload() override {
        return 50.0 - emptyWeight;
    }
    double minEmptyWeight() override {
        return 10.0;
    }
    double maxEmptyWeight() override {
        return 40.0;
    }
    double minWingArea() override {
        return 10;
    }
    double maxWingArea() override {
        return 961;
    }
    double minCoL() override {
        return 0.5;
    }
    double maxCoL() override {
        return 2.0;
    }
    double maxCruise() override {
        return 50;
    }
    double maxThrust() override {
        return 100;
    }
    double calculateScore() override {
        double requiredLift = emptyWeight + payload;
        double availableLift = CoL * wingArea;
        if(requiredLift > availableLift) return 0;

        double requiredCruiseSpeed = requiredLift / /* (fluid dynamic pressure q) */ wingArea;
        if(requiredCruiseSpeed > maxCruise()) return 0;

        // drag scales with square of speed and area times CoL?
        double drag = (requiredCruiseSpeed * requiredCruiseSpeed) * wingArea * CoL;
        if(drag > maxThrust()) return 0;

        double score = payload * requiredCruiseSpeed;
        return score;
    }
};

int main() {
    double stride = 0.1;
    double maxScore = 0;
    M2Plane plane;

    std::cout << "weight\tpayload\tCoL\tarea\tscore\n";
    for(plane.emptyWeight = plane.minEmptyWeight(); plane.emptyWeight < plane.maxEmptyWeight(); plane.emptyWeight+=stride) {
        for (plane.payload = plane.minPayload(); plane.payload < plane.maxPayload(); plane.payload+=stride) {
            for (plane.CoL = plane.minCoL(); plane.CoL < plane.maxCoL(); plane.CoL+=stride) {
                for (plane.wingArea = plane.minWingArea(); plane.wingArea < plane.maxWingArea(); plane.wingArea+=stride) {
                    double score = plane.calculateScore();
                    if (score > maxScore) {
                        maxScore = score;
                        std::cout << "empty weight:" << plane.emptyWeight << "\t"
                            << "payload: " << plane.payload << "\t"
                            << "CoL: " << plane.CoL << "\t"
                            << "wing area: " << plane.wingArea << "\t" 
                            << "score: " << score << "\n";
                    }
                }
            }
        }
    }
    std::cout << "\n\n Max Score: " << maxScore << std::endl;
}