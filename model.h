#pragma once
//#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class DecisionTree {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        if (x[0] <= 1.5) {
                            if (x[1] <= 6.0) {
                                return 3;
                            }

                            else {
                                return 0;
                            }
                        }

                        else {
                            if (x[0] <= 2.5) {
                                return 1;
                            }

                            else {
                                return 2;
                            }
                        }
                    }

                    /**
                    * Predict readable class name
                    */
                    const char* predictLabel(float *x) {
                        return idxToLabel(predict(x));
                    }

                    /**
                    * Convert class idx to readable name
                    */
                    const char* idxToLabel(uint8_t classIdx) {
                        switch (classIdx) {
                            case 0:
                            return "Dataset Normal";
                            case 1:
                            return "Dataset Tachypnea";
                            case 2:
                            return "Dataset Bradypnea";
                            case 3:
                            return "Dataset tidak bernapas";
                            default:
                            return "Houston we have a problem";
                        }
                    }

                protected:
                };
            }
        }
    }
