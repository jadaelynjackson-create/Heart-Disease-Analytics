#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cmath>
using namespace std;
// ================================================================
// Heart Disease Analytics Program
// ---------------------------------------------------------------
// This program is made for a C++ class project.
// It reads heart-disease style CSV data and predicts the chance
// that a patient may have heart disease based on common columns:
//
// age, sex, cp, trestbps, chol, fbs, restecg, thalach,
// exang, oldpeak, slope, ca, thal, target
//
// IMPORTANT:
// This program is for school analytics practice only.
// It is NOT medical advice and should not be used to diagnose anyone.
// ================================================================
// ---------------------------------------------------------------
// Meaning of the CSV columns:
// age      = age in years
// sex      = 1 male, 0 female
// cp       = chest pain type, 0-3
// trestbps = resting blood pressure
// chol     = cholesterol
// fbs      = fasting blood sugar above 120 mg/dl, 1 true, 0 false
// restecg  = resting ECG result, 0-2
// thalach  = maximum heart rate achieved
// exang    = exercise induced angina, 1 yes, 0 no
// oldpeak  = ST depression caused by exercise
// slope    = slope of peak exercise ST segment, 0-2
// ca       = number of major vessels, 0-4
// thal     = thalassemia value, 0-3
// target   = actual dataset result, 1 disease, 0 no disease
// ---------------------------------------------------------------
class PatientRecord
{
private:
    int age;
    int sex;
    int cp;
    int trestbps;
    int chol;
    int fbs;
    int restecg;
    int thalach;
    int exang;
    double oldpeak;
    int slope;
    int ca;
    int thal;
    int target;
public:
    PatientRecord()
    {
        age = 0;
        sex = 0;
        cp = 0;
        trestbps = 0;
        chol = 0;
        fbs = 0;
        restecg = 0;
        thalach = 0;
        exang = 0;
        oldpeak = 0.0;
        slope = 0;
        ca = 0;
        thal = 0;
        target = -1;
    }
    PatientRecord(int a, int s, int chest, int bp, int cholesterol,
        int sugar, int ecg, int heartRate, int angina,
        double depression, int stSlope, int vessels,
        int thalValue, int actualTarget)
    {
        age = a;
        sex = s;
        cp = chest;
        trestbps = bp;
        chol = cholesterol;
        fbs = sugar;
        restecg = ecg;
        thalach = heartRate;
        exang = angina;
        oldpeak = depression;
        slope = stSlope;
        ca = vessels;
        thal = thalValue;
        target = actualTarget;
    }
    int getAge() const
    {
        return age;
    }
    int getSex() const
    {
        return sex;
    }
    int getCp() const
    {
        return cp;
    }
    int getTrestbps() const
    {
        return trestbps;
    }
    int getChol() const
    {
        return chol;
    }
    int getFbs() const
    {
        return fbs;
    }
    int getRestecg() const
    {
        return restecg;
    }
    int getThalach() const
    {
        return thalach;
    }
    int getExang() const
    {
        return exang;
    }
    double getOldpeak() const
    {
        return oldpeak;
    }
    int getSlope() const
    {
        return slope;
    }
    int getCa() const
    {
        return ca;
    }
    int getThal() const
    {
        return thal;
    }
    int getTarget() const
    {
        return target;
    }
    void printShortRow(int rowNumber) const
    {
        cout << left
            << setw(6) << rowNumber
            << setw(6) << age
            << setw(6) << sex
            << setw(6) << cp
            << setw(10) << trestbps
            << setw(8) << chol
            << setw(9) << thalach
            << setw(8) << exang
            << setw(9) << oldpeak
            << setw(7) << ca
            << setw(7) << thal
            << setw(8) << target
            << endl;
    }
};
class RiskResult
{
private:
    int score;
    double percent;
    string category;
    vector<string> reasons;
public:
    RiskResult()
    {
        score = 0;
        percent = 0.0;
        category = "Unknown";
    }
    void setScore(int s)
    {
        score = s;
    }
    void setPercent(double p)
    {
        percent = p;
    }
    void setCategory(string c)
    {
        category = c;
    }
    void addReason(string reason)
    {
        reasons.push_back(reason);
    }
    int getScore() const
    {
        return score;
    }
    double getPercent() const
    {
        return percent;
    }
    string getCategory() const
    {
        return category;
    }
    vector<string> getReasons() const
    {
        return reasons;
    }
    void print() const
    {
        cout << fixed << setprecision(1);
        cout << "\nEstimated Heart Disease Likelihood: " << percent << "%\n";
        cout << "Risk Category: " << category << endl;
        cout << "Point Score: " << score << endl;
        cout << "\nMain Factors Found:\n";
        if (reasons.empty())
        {
            cout << "- No major risk factors were triggered by this rule set.\n";
        }
        else
        {
            for (string reason : reasons)
            {
                cout << "- " << reason << endl;
            }
        }
    }
};
class HeartDiseaseAnalyzer
{
private:
    vector<PatientRecord> records;
    vector<string> splitCSVLine(string line) const
    {
        vector<string> parts;
        string item;
        stringstream ss(line);
        while (getline(ss, item, ','))
        {
            parts.push_back(item);
        }
        return parts;
    }
    bool isHeaderLine(string line) const
    {
        return line.find("age") != string::npos &&
            line.find("sex") != string::npos &&
            line.find("target") != string::npos;
    }
    double clamp(double value, double low, double high) const
    {
        if (value < low)
        {
            return low;
        }
        if (value > high)
        {
            return high;
        }
        return value;
    }
public:
    void addRecord(PatientRecord record)
    {
        records.push_back(record);
    }
    int size() const
    {
        return static_cast<int>(records.size());
    }
    bool loadFromCSV(string filename)
    {
        ifstream file(filename);
        if (!file)
        {
            return false;
        }
        string line;
        while (getline(file, line))
        {
            if (line.size() == 0)
            {
                continue;
            }
            if (isHeaderLine(line))
            {
                continue;
            }
            vector<string> p = splitCSVLine(line);
            if (p.size() != 14)
            {
                continue;
            }
            int age = stoi(p[0]);
            int sex = stoi(p[1]);
            int cp = stoi(p[2]);
            int trestbps = stoi(p[3]);
            int chol = stoi(p[4]);
            int fbs = stoi(p[5]);
            int restecg = stoi(p[6]);
            int thalach = stoi(p[7]);
            int exang = stoi(p[8]);
            double oldpeak = stod(p[9]);
            int slope = stoi(p[10]);
            int ca = stoi(p[11]);
            int thal = stoi(p[12]);
            int target = stoi(p[13]);
            PatientRecord record(age, sex, cp, trestbps, chol, fbs,
                restecg, thalach, exang, oldpeak,
                slope, ca, thal, target);
            addRecord(record);
        }
        return true;
    }
    RiskResult predictRisk(const PatientRecord& patient) const
    {
        RiskResult result;
        int score = 0;
        if (patient.getAge() >= 60)
        {
            score += 2;
            result.addReason("Age is 60 or older");
        }
        else if (patient.getAge() >= 50)
        {
            score += 1;
            result.addReason("Age is between 50 and 59");
        }
        if (patient.getSex() == 1)
        {
            score += 1;
            result.addReason("Male sex is counted as one risk point in this model");
        }
        if (patient.getCp() == 0)
        {
            score += 3;
            result.addReason("Chest pain type is 0, which is often high risk in this dataset");
        }
        else if (patient.getCp() == 1)
        {
            score += 1;
            result.addReason("Chest pain type adds slight risk");
        }
        if (patient.getTrestbps() >= 160)
        {
            score += 3;
            result.addReason("Resting blood pressure is 160 or higher");
        }
        else if (patient.getTrestbps() >= 140)
        {
            score += 2;
            result.addReason("Resting blood pressure is 140 or higher");
        }
        else if (patient.getTrestbps() >= 130)
        {
            score += 1;
            result.addReason("Resting blood pressure is 130 or higher");
        }
        if (patient.getChol() >= 300)
        {
            score += 2;
            result.addReason("Cholesterol is 300 or higher");
        }
        else if (patient.getChol() >= 240)
        {
            score += 1;
            result.addReason("Cholesterol is 240 or higher");
        }
        if (patient.getFbs() == 1)
        {
            score += 1;
            result.addReason("Fasting blood sugar is above 120 mg/dl");
        }
        if (patient.getRestecg() == 2)
        {
            score += 1;
            result.addReason("Resting ECG value is abnormal/high category");
        }
        if (patient.getThalach() < 120)
        {
            score += 3;
            result.addReason("Maximum heart rate is below 120");
        }
        else if (patient.getThalach() < 140)
        {
            score += 2;
            result.addReason("Maximum heart rate is below 140");
        }
        else if (patient.getThalach() < 155)
        {
            score += 1;
            result.addReason("Maximum heart rate is below 155");
        }
        if (patient.getExang() == 1)
        {
            score += 3;
            result.addReason("Exercise-induced angina is present");
        }
        if (patient.getOldpeak() >= 3.0)
        {
            score += 3;
            result.addReason("Oldpeak is 3.0 or higher");
        }
        else if (patient.getOldpeak() >= 2.0)
        {
            score += 2;
            result.addReason("Oldpeak is 2.0 or higher");
        }
        else if (patient.getOldpeak() >= 1.0)
        {
            score += 1;
            result.addReason("Oldpeak is 1.0 or higher");
        }
        if (patient.getSlope() == 0)
        {
            score += 2;
            result.addReason("ST slope value is 0");
        }
        else if (patient.getSlope() == 1)
        {
            score += 1;
            result.addReason("ST slope value is 1");
        }
        if (patient.getCa() >= 3)
        {
            score += 3;
            result.addReason("Number of major vessels is 3 or more");
        }
        else if (patient.getCa() >= 1)
        {
            score += 2;
            result.addReason("Number of major vessels is at least 1");
        }
        if (patient.getThal() == 3)
        {
            score += 3;
            result.addReason("Thal value is 3");
        }
        else if (patient.getThal() == 1)
        {
            score += 1;
            result.addReason("Thal value is 1");
        }
        double percent = (score / 27.0) * 100.0;
        percent = clamp(percent, 1.0, 99.0);
        string category;
        if (percent < 25)
        {
            category = "Low";
        }
        else if (percent < 50)
        {
            category = "Moderate";
        }
        else if (percent < 75)
        {
            category = "High";
        }
        else
        {
            category = "Very High";
        }
        result.setScore(score);
        result.setPercent(percent);
        result.setCategory(category);
        return result;
    }
    void printDatasetPreview(int maxRows) const
    {
        cout << "\nDataset Preview\n";
        cout << "---------------------------------------------------------------\n";
        cout << left
            << setw(6) << "Row"
            << setw(6) << "Age"
            << setw(6) << "Sex"
            << setw(6) << "CP"
            << setw(10) << "BP"
            << setw(8) << "Chol"
            << setw(9) << "HR"
            << setw(8) << "Exang"
            << setw(9) << "Oldpeak"
            << setw(7) << "CA"
            << setw(7) << "Thal"
            << setw(8) << "Target"
            << endl;
        cout << "---------------------------------------------------------------\n";
        int limit = min(maxRows, static_cast<int>(records.size()));
        for (int i = 0; i < limit; i++)
        {
            records[i].printShortRow(i + 1);
        }
        cout << "---------------------------------------------------------------\n";
    }
    void printSummaryStats() const
    {
        if (records.empty())
        {
            cout << "\nNo records loaded.\n";
            return;
        }
        double totalAge = 0;
        double totalBP = 0;
        double totalChol = 0;
        double totalHR = 0;
        int diseaseCount = 0;
        int noDiseaseCount = 0;
        for (const PatientRecord& r : records)
        {
            totalAge += r.getAge();
            totalBP += r.getTrestbps();
            totalChol += r.getChol();
            totalHR += r.getThalach();
            if (r.getTarget() == 1)
            {
                diseaseCount++;
            }
            else if (r.getTarget() == 0)
            {
                noDiseaseCount++;
            }
        }
        double n = static_cast<double>(records.size());
        cout << fixed << setprecision(2);
        cout << "\nHeart Disease Dataset Summary\n";
        cout << "---------------------------------------------------------------\n";
        cout << "Total records: " << records.size() << endl;
        cout << "Average age: " << totalAge / n << endl;
        cout << "Average resting blood pressure: " << totalBP / n << endl;
        cout << "Average cholesterol: " << totalChol / n << endl;
        cout << "Average max heart rate: " << totalHR / n << endl;
        cout << "Patients with heart disease target = 1: " << diseaseCount << endl;
        cout << "Patients without heart disease target = 0: " << noDiseaseCount << endl;
        cout << "---------------------------------------------------------------\n";
    }
    void testAccuracyAgainstDataset() const
    {
        if (records.empty())
        {
            cout << "\nNo records loaded.\n";
            return;
        }
        int correct = 0;
        for (const PatientRecord& r : records)
        {
            RiskResult prediction = predictRisk(r);
            int predictedTarget;
            if (prediction.getPercent() >= 50.0)
            {
                predictedTarget = 1;
            }
            else
            {
                predictedTarget = 0;
            }
            if (predictedTarget == r.getTarget())
            {
                correct++;
            }
        }
        double accuracy = (static_cast<double>(correct) / records.size()) * 100.0;
        cout << fixed << setprecision(2);
        cout << "\nRule-Based Model Accuracy Check\n";
        cout << "---------------------------------------------------------------\n";
        cout << "Correct predictions: " << correct << " out of " << records.size() << endl;
        cout << "Estimated accuracy on loaded file: " << accuracy << "%\n";
        cout << "Note: This is a simple school project model, not true AI.\n";
        cout << "---------------------------------------------------------------\n";
    }
    PatientRecord enterPatientManually() const
    {
        int age;
        int sex;
        int cp;
        int trestbps;
        int chol;
        int fbs;
        int restecg;
        int thalach;
        int exang;
        double oldpeak;
        int slope;
        int ca;
        int thal;
        cout << "\nEnter Patient Stats\n";
        cout << "Age: ";
        cin >> age;
        cout << "Sex, 1 male / 0 female: ";
        cin >> sex;
        cout << "Chest pain type cp, 0-3: ";
        cin >> cp;
        cout << "Resting blood pressure trestbps: ";
        cin >> trestbps;
        cout << "Cholesterol chol: ";
        cin >> chol;
        cout << "Fasting blood sugar fbs, 1 true / 0 false: ";
        cin >> fbs;
        cout << "Resting ECG restecg, 0-2: ";
        cin >> restecg;
        cout << "Max heart rate thalach: ";
        cin >> thalach;
        cout << "Exercise angina exang, 1 yes / 0 no: ";
        cin >> exang;
        cout << "Oldpeak: ";
        cin >> oldpeak;
        cout << "Slope, 0-2: ";
        cin >> slope;
        cout << "Number of major vessels ca, 0-4: ";
        cin >> ca;
        cout << "Thal, 0-3: ";
        cin >> thal;
        return PatientRecord(age, sex, cp, trestbps, chol, fbs,
            restecg, thalach, exang, oldpeak,
            slope, ca, thal, -1);
    }
};
void printProgramIntro()
{
    cout << "===============================================================\n";
    cout << "              HEART DISEASE ANALYTICS PROGRAM\n";
    cout << "===============================================================\n";
    cout << "This program loads heart disease CSV data and estimates\n";
    cout << "how likely a patient may be to have heart disease.\n";
    cout << "This is for C++ analytics practice only.\n";
    cout << "===============================================================\n";
}
void printMenu()
{
    cout << "\nMenu\n";
    cout << "1. Show dataset preview\n";
    cout << "2. Show dataset summary analytics\n";
    cout << "3. Test rule-based model on dataset\n";
    cout << "4. Enter new patient and predict risk\n";
    cout << "5. Exit\n";
    cout << "Choose: ";
}
int main()
{
    HeartDiseaseAnalyzer analyzer;
    string filename = "heart.csv";
    printProgramIntro();
    cout << "\nTrying to load file named: " << filename << endl;
    bool loaded = analyzer.loadFromCSV(filename);
    if (!loaded)
    {
        cout << "Could not open heart.csv.\n";
        cout << "Make sure your CSV file is in the same folder as this program.\n";
        cout << "The first line should be:\n";
        cout << "age,sex,cp,trestbps,chol,fbs,restecg,thalach,exang,oldpeak,slope,ca,thal,target\n";
        return 0;
    }
    cout << "File loaded successfully.\n";
    cout << "Records loaded: " << analyzer.size() << endl;
    int choice = 0;
    while (choice != 5)
    {
        printMenu();
        cin >> choice;
        if (choice == 1)
        {
            analyzer.printDatasetPreview(15);
        }
        else if (choice == 2)
        {
            analyzer.printSummaryStats();
        }
        else if (choice == 3)
        {
            analyzer.testAccuracyAgainstDataset();
        }
        else if (choice == 4)
        {
            PatientRecord patient = analyzer.enterPatientManually();
            RiskResult result = analyzer.predictRisk(patient);
            result.print();
        }
        else if (choice == 5)
        {
            cout << "\nExiting program.\n";
        }
        else
        {
            cout << "\nInvalid choice. Try again.\n";
        }
    }
    return 0;
}

