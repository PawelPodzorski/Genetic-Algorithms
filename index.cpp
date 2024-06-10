#include <bits/stdc++.h>

using namespace std;


struct Solutions{
    double score, x, y;

    void fitness()
    {
        double answer = 1 * x + 2 * y -5;

        score = ((answer == 0) ? (-10) : (abs((1/answer))));
    }
};



int main(){
    // Generator losowych liczb
    // random_device device;
    // alternatywa
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(-100, 100);
    vector<Solutions> solutions;

    const int NUM_GEN = 1'000; // liczba genow
    const int GENERATIONS_SIZE = 1000; // liczba generacji
    const int SAMPLE_SIZE = 100; // liczba poczatkowych genow ktore beda mutowac (musi byc co najmniej 2 razy mniejsze od [NUM_GEN])

    double best = INT_MIN; // Najlepszy [score] gena do porownywania czy nastapil progres w generacji
    string best_string = "";
    
    // Pierwsza generacja z losowymi liczbami
    for(int i=0; i < NUM_GEN; i++){ 
        solutions.push_back(Solutions{
            0,
            double(dist(gen)),
            double(dist(gen))
        });
    }


    for(int generation = 0; generation <= GENERATIONS_SIZE; generation++){
        // ocenianie genow i przypisywanie im [score]
        for(auto& s : solutions){ s.fitness(); }

        // sortowanie po najwiekszym [score]
        sort(solutions.begin(), solutions.end(), [](const auto& l,const auto& r){ return l.score > r.score; });

        //
        // Jesli odpowiedz do [answer] jest perfekcyjnie prawidlowa
        if(solutions[solutions.size() - 1].score == -10){
            best_string = " Perfect score - [GEN] => " + to_string(generation) + " { x: " + to_string(solutions[solutions.size() - 1].x) + " ; y: " + to_string(solutions[solutions.size() - 1].y) + " }";
            
            cout << best_string << "\n";

            // END
            system("pause");
            return 0;
        }
        //

        // wyswietlanie najlepszego genu w danej generacji
        ///*
        cout << fixed
            << "GENERATION ===[ " << generation << " ]===\n"
            << "Score: " << solutions[0].score
            << "\nx: " << solutions[0].x << " y: " << solutions[0].y << "\n\n";
        //*/
        //

        
        vector<Solutions> sample;
        // kopiowanie [SAMPLE_SIZE] ilosci poczatkowych genow do mutacji
        copy(solutions.begin(), solutions.begin() + SAMPLE_SIZE, back_inserter(sample));

        if(sample[0].score > best){
            best = sample[0].score;
            best_string = to_string(best) + " [GEN] => " + to_string(generation) + " { x: " + to_string(sample[0].x) + " ; y: " + to_string(sample[0].y) + " }";
        }

        solutions.clear(); // Usuwanie poprzedniej generacji



        // Najlepsze [SAMPLE_SIZE] genow przezywa do nastepnej generacji
        copy(sample.begin(), sample.end(), back_inserter(solutions));

        // przeprowadzanie mutacji
        uniform_real_distribution<double> mutation_dist(0.9999,1.0001); // odchylenie zmutowanego genu od poprzednika

        // mutowanie najlepszych genow z poprzedniej generacji
        for(auto& s : sample){
            s.x *= mutation_dist(gen);
            s.y *= mutation_dist(gen);
        }

        // zmutowanie geny przechodza niezmienione do nastepnej generacji
        copy(sample.begin(), sample.end(), back_inserter(solutions));
        
        // reszta genow to mieszanka [x] i [y] od dwoch roznych zmutownych rodzicow
        uniform_int_distribution<int> cross(0,SAMPLE_SIZE-1);
        for(int i = 0; i < NUM_GEN - SAMPLE_SIZE*2; i++){ // zapelnianie reszty genow
            solutions.push_back(Solutions{ // branie [x] od jednego zmutowanego rodzica i [y] od drugiego
                0,
                sample[cross(gen)].x,
                sample[cross(gen)].y
            });
        }
    }

    cout << best_string << "\n";


    // END
    system("pause");
    return 0;
}