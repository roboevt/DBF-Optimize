#include <iostream>
#include <vector>

double calculateScoreM2(double payload, double emptyWeight, double wingArea, double CoL) {
    double requiredLift = emptyWeight + payload;
    double availableLift = CoL * wingArea;
    if(requiredLift > availableLift) return 0;
    // TODO find out max thrust/pitch speed and limit cruise
    
    double requiredCruiseSpeed = requiredLift / /* (fluid dynamic pressure q) */ wingArea;
    double maxThrust = 100;
    // drag scales with square of speed and area times CoL?
    double drag = (requiredCruiseSpeed * requiredCruiseSpeed) * wingArea * CoL;
    if(drag > maxThrust) return 0;

    double score = payload * requiredCruiseSpeed;
    return score;
}

//calculate bounds/constraints on input variables
double minEmptyWeight() {
    return 10.0;
}
double maxEmptyWeight() {
    return 40.0;
}
double minPayload() {
    return 1.0;
}
double maxPayload(double emptyWeight) {
    return 50.0 - emptyWeight;
}
double minWingArea() {
    return 1;
}
double maxWingArea() {
    return 961;
}
double minCoL() {
    return 0.5;
}
double maxCol() {
    return 2.0;
}

int main() {
    double stride = 0.1;
    double maxScore = 0;

    std::cout << "weight\tpayload\tCoL\tarea\tscore\n";
    for(double emptyWeight = minEmptyWeight(); emptyWeight < maxEmptyWeight(); emptyWeight+=stride) {
        for (double payload = minPayload(); payload < maxPayload(emptyWeight); payload+=stride) {
            for (double CoL = minCoL(); CoL < maxCol(); CoL+=stride) {
                for (double wingArea = minWingArea(); wingArea < maxWingArea(); wingArea+=stride) {
                    double score = calculateScoreM2(payload, emptyWeight, wingArea, CoL);
                    if (score > maxScore) {
                        maxScore = score;
                        std::cout << emptyWeight << "\t"
                            << payload << "\t"
                            << CoL << "\t"
                            << wingArea << "\t" 
                            << score << "\n";
                    }
                }
            }
        }
    }
    std::cout << "\n\n Max Score: " << maxScore << std::endl;
}