// champions league Calculator prediction
// CMPG Project
#include<iostream>
#include<string>
#include<iomanip>
#include<limits>
#include<cmath>
#include<fstream>

using namespace std;

// Define struct to hold all relevant data for a single team in the tournament
struct Tournament
{
    string name;
    string leagues;

    // Historical group stage wins (W) over 8 seasons (2015/16 to 2022/23).
    int games_groupstage[1][8];

    // Historical knockout stage wins (W) over 8 seasons (2015/16 to 2022/23).
    int games_knockouts[1][8];

    // Historical domestic league wins (W) over 4 seasons (2019/20 to 2022/23).
    // Used as a proxy for team strength in the later tournament stages (semi-finals).
    int games_semi_finals[1][4];

    // Historical domestic league wins (W) over 4 seasons (2019/20 to 2022/23).
    // Used as a proxy for team strength in the final.
    int games_finals[1][4];

    // Probability of a team winning a group stage match, calculated from historical data.
    double group_stage_probabilities[32][1];

    // Probability of a team winning a knockout stage tie, calculated from historical data.
    double knockouts_stage_probabilities[32][1];

    // Average probability (based on league wins) used for semi-final prediction.
    double semi_finals_probabilities[32][1];

    // Average probability (based on league wins) used for final prediction.
    double finals_probabilities[32][1];
};

// Function Prototypes

// Calculates and displays the probability of a team advancing past the group stages.
void probabilities_groupStages_advancing(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position);

// Calculates and displays the probability of a team being eliminated from the group stages.
void probabilities_groupStages_elimination(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position);

// Calculates and displays the probability of a team advancing past the knockout round.
void probabilities_knockouts_advancing(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position);

// Calculates and displays the probability of a team being eliminated from the knockout round.
void probabilities_knockouts_elimination(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position);

// Calculates and displays the probability of a team advancing past the semi-finals (uses league wins as a proxy).
void probabilities_semi_finals_advancing(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position);

// Calculates and displays the probability of a team being eliminated from the semi-finals (uses league losses as a proxy).
void probabilities_semi_finals_elimination(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position);

// Determines and displays the predicted winner of the final between two selected teams.
void probabilities_finals(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position, string opponent, int position_second);

// Displays the main menu, which includes the list of all 32 teams grouped by their Champions League group.
void menu(Tournament teams[], int SIZE);

// Displays the sub-menu of statistical analysis options for the chosen team.
void teamMenu(Tournament teams[], int SIZE, string user_choice);

// Calculates the binomial probability for a team advancing from the group stage.
// p_value: probability of success (win), n: number of trials (6 group games).
// Success is defined as winning k_min=4 or more games.
double binomial_function_groupstage(double p_value, int n);

// Calculates the binomial probability for a team advancing from the knockout stage.
// p_value: probability of success (advancing), n: number of trials (2 games in a tie).
// Success is defined as winning k_min=2 or more games (winning the tie).
double binomial_function_knockouts(double p_value, int n);

// Calculates the binomial probability for a team advancing from the semi-finals.
// p_value: probability of success (win rate based on league wins), n: number of trials (2 games in the semi-final tie).
// Success is defined as winning k_min=2 or more games (winning the tie).
double binomial_function_semi_finals(double p_value, int n);



int main()
{
    const int SIZE = 32;          // The total number of teams in the Champions League group stage
    string user_choice = "";      // Stores the team name entered by the user
    int stats_choice = 0;         // Stores the user's choice from the statistical analysis menu (1-7)
    char continue_program = ' ';  // Stores user input for continuing/exiting the program

    Tournament teams[SIZE];       // Array of structures to hold data for all 32 teams

    ////////
    // I/O
    ////////

    // Reading team names from a text file. Array holding the names of all 32 participating teams
    string TeamNames[SIZE];
    int index;
    ifstream inputFile;

    // open the file
    inputFile.open("TeamNames.txt");

    // test if file successfully open
    if (inputFile){
        while (index < SIZE && getline(inputFile, TeamNames[index]))
            index++;
    }

    else {
        cout << "Error opening the file" << endl;
    }

    inputFile.close();

    // 3D array storing historical group stage wins (W) over 8 seasons (2015/16 to 2022/23).
    int playedMatches_groupStage[SIZE][1][8] = {
                                    {{0, 0, 0, 0, 0, 0, 0, 1}}, // Antwerp
                                    {{3, 4, 0, 0, 0, 0, 0, 0}}, // Arsenal
                                    {{5, 4, 4, 4, 2, 2, 1, 0}}, // Atletico Madrid
                                    {{4, 5, 4, 4, 5, 5, 2, 2}}, // Barcelona
                                    {{5, 4, 5, 4, 6, 6, 6, 6}}, // Bayern
                                    {{3, 0, 0, 0, 2, 2, 4, 0}}, // Benfica
                                    {{0, 0, 0, 0, 0, 0, 1, 0}}, // Braga
                                    {{0, 3, 0, 0, 0, 0, 0, 0}}, // Celtic
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Copenhagen
                                    {{0, 0, 0, 1, 0, 0, 0, 0}}, // Crvena zvezda
                                    {{4, 4, 0, 4, 3, 3, 2, 0}}, // Dortmund
                                    {{0, 0, 0, 0, 0, 0, 2, 0}}, // Feyenoord
                                    {{0, 0, 0, 0, 0, 0, 1, 0}}, // Galatasaray
                                    {{0, 0, 0, 2, 1, 3, 3, 0}}, // Inter
                                    {{0, 0, 0, 0, 2, 2, 4, 0}}, // Lazio
                                    {{0, 0, 0, 0, 0, 0, 1, 0}}, // Lens
                                    {{4, 2, 5, 4, 4, 5, 4, 4}}, // Man City
                                    {{3, 0, 5, 3, 0, 3, 3, 0}}, // Man United
                                    {{0, 0, 0, 0, 0, 0, 3, 0}}, // Milan
                                    {{0, 0, 4, 2, 3, 3, 5, 0}}, // Napoli
                                    {{0, 0, 0, 0, 0, 0, 1, 0}}, // Newcastle
                                    {{3, 2, 3, 5, 3, 4, 1, 4}}, // Porto
                                    {{5, 4, 5, 5, 5, 4, 3, 4}}, // PSG
                                    {{2, 0, 0, 0, 0, 0, 0, 2}}, // PSV
                                    {{0, 0, 0, 0, 3, 4, 1, 4}}, // RB Leipzig
                                    {{0, 0, 0, 0, 2, 3, 1, 0}}, // RB Salzburg
                                    {{5, 3, 4, 4, 3, 3, 5, 4}}, // Real Madrid
                                    {{0, 0, 0, 0, 0, 0, 3, 0}}, // Real Sociedad
                                    {{2, 3, 2, 3, 5, 4, 1, 2}}, // Sevilla
                                    {{3, 2, 2, 3, 1, 2, 0, 0}}, // Shakhtar Donetsk
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Union Berlin
                                    {{0, 0, 0, 0, 1, 0, 1, 0}}  // Young Boys
                                    };


    // 3D array storing historical knockout stage wins (W) over 8 seasons (2015/16 to 2022/23).
    // A 'win' here means advancing past the round of 16, quarter-finals, or semi-finals.
    // Each number represents advancing in that specific round for a season (0=not reached/eliminated, 1=advanced).
    int playedMatches_knockouts[SIZE][1][8] = {
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Antwerp
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Arsenal
                                    {{1, 1, 0, 0, 1, 0, 0, 0}}, // Atletico Madrid
                                    {{1, 1, 1, 1, 1, 0, 0, 0}}, // Barcelona
                                    {{1, 1, 1, 0, 1, 1, 1, 1}}, // Bayern
                                    {{1, 0, 0, 0, 0, 0, 1, 1}}, // Benfica
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Braga
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Celtic
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Copenhagen
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Crvena zvezda
                                    {{0, 1, 0, 1, 0, 1, 0, 1}}, // Dortmund
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Feyenoord
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Galatasaray
                                    {{0, 0, 0, 0, 0, 0, 1, 0}}, // Inter
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Lazio
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Lens
                                    {{1, 0, 1, 1, 1, 1, 1, 1}}, // Man City
                                    {{0, 0, 1, 0, 0, 0, 0, 0}}, // Man United
                                    {{0, 0, 0, 0, 0, 0, 1, 0}}, // Milan
                                    {{0, 0, 0, 0, 0, 0, 1, 0}}, // Napoli
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Newcastle
                                    {{0, 1, 0, 1, 1, 1, 0, 0}}, // Porto
                                    {{1, 0, 0, 0, 1, 0, 0, 0}}, // PSG
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // PSV
                                    {{0, 0, 0, 0, 0, 1, 0, 0}}, // RB Leipzig
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // RB Salzburg
                                    {{1, 1, 1, 0, 0, 1, 1, 1}}, // Real Madrid
                                    {{0, 0, 0, 0, 0, 0, 1, 0}}, // Real Sociedad
                                    {{0, 1, 1, 0, 0, 0, 0, 0}}, // Sevilla
                                    {{0, 0, 0, 1, 1, 1, 0, 0}}, // Shakhtar Donetsk
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}, // Union Berlin
                                    {{0, 0, 0, 0, 0, 0, 0, 0}}  // Young Boys
                                    };
    // history records from 2019/20 to 2022/23, hence 4 seasons
    // 3D array storing domestic league wins (W) over 4 seasons (2019/20 to 2022/23).
    // This data is used as a proxy for team strength in the semi-finals and final.
     int playedMatches_wins_league[SIZE][1][4] = {
                                    {{15, 18, 16, 21}}, // Antwerp
                                    {{14, 18, 22, 20}}, // Arsenal
                                    {{18, 26, 21, 23}}, // Atletico Madrid
                                    {{25, 24, 21, 28}}, // Barcelona
                                    {{26, 24, 24, 23}}, // Bayern
                                    {{27, 24, 25, 28}}, // Benfica
                                    {{23, 24, 25, 25}}, // Braga
                                    {{26, 29, 28, 28}}, // Celtic
                                    {{17, 21, 18, 22}}, // Copenhagen
                                    {{30, 33, 31, 33}}, // Crvena zvezda
                                    {{24, 21, 22, 23}}, // Dortmund
                                    {{15, 21, 18, 25}}, // Feyenoord
                                    {{21, 25, 23, 30}}, // Galatasaray
                                    {{22, 28, 25, 26}}, // Inter
                                    {{24, 21, 21, 20}}, // Lazio
                                    {{11, 15, 15, 22}}, // Lens
                                    {{27, 27, 29, 28}}, // Man City
                                    {{18, 21, 16, 16}}, // Man United
                                    {{16, 20, 26, 23}}, // Milan
                                    {{20, 24, 23, 28}}, // Napoli
                                    {{11, 12, 11, 13}}, // Newcastle
                                    {{27, 26, 29, 29}}, // Porto
                                    {{27, 26, 27, 27}}, // PSG
                                    {{22, 25, 22, 25}}, // PSV
                                    {{21, 20, 22, 18}}, // RB Leipzig
                                    {{23, 25, 25, 25}}, // RB Salzburg
                                    {{26, 25, 26, 24}}, // Real Madrid
                                    {{17, 19, 17, 20}}, // Real Sociedad
                                    {{21, 21, 21, 17}}, // Sevilla
                                    {{23, 25, 0, 21}},  // Shakhtar Donetsk
                                    {{11, 11, 11, 11}}, // Union Berlin
                                    {{21, 26, 25, 21}}  // Young Boys
                                    };

    // Array containing the domestic league name for each team.

    ////////
    // I/O
    ////////

    // Reading league names from a text file. Array holding the names of all the leagues
    string european_leagues[SIZE];
    int count = 0;
    ifstream inputFile2;

    // open the file
    inputFile2.open("leagues.txt");

    // test if file successfully open
    if (inputFile2){
        while (count < SIZE && getline(inputFile2, european_leagues[count]))
            count++;
    }

    else {
        cout << "Error opening the file" << endl;
    }

    inputFile2.close();

    // Copy TeamName to struct name
    for (int i = 0; i < SIZE; i++)
    {
        teams[i].name = TeamNames[i];
    }


    // copy european_league to struct league
    for (int i = 0; i < SIZE; i++)
    {
        teams[i].leagues = european_leagues[i];
    }


    // Copy playedMatches_groupStage to struct games_groupstage
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            teams[i].games_groupstage[0][k] = playedMatches_groupStage[i][0][k];
        }
    }

    // Copy playedMatches_knockouts to struct games_knockouts
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            teams[i].games_knockouts[0][k] = playedMatches_knockouts[i][0][k];
        }
    }

    // copy playedMatches_wins_league to struct semi-finals
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            teams[i].games_semi_finals[0][k] = playedMatches_wins_league[i][0][k];
        }
    }

    // copy playedMatches_wins_league to struct finals
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            teams[i].games_finals[0][k] = playedMatches_wins_league[i][0][k];
        }
    }

    /////////////////////////////
    // The start of the program
    ////////////////////////////

    do
    {
        cout << endl;
        // showing the menu and the list of all teams
        menu(teams, SIZE);
        cout << endl;
        cout << "Enter the name of the team (ensure you enter the exact name): ";
        getline(cin, user_choice); // Read the user's selected team name

        // check if the user entered the right team name
        bool found = false;
        int position = -1;
        int index = 0;

        // Loop to search for the entered team name in the 'teams' array
        while (!found && index < SIZE)
        {
            if (teams[index].name == user_choice){
                found = true;
                position = index;
            }

            index++;
        }

        // Error message if the team name was not found
        if (found == false){
            cout << "\nError! Club name not found."
            << endl;
        }

        // Proceed if the team name was found
        else{

            // Display introductory description of the statistical analysis method (Binomial Distribution)
            cout << endl;
            cout << "\t\t\t " << user_choice << " | Statistical analysis" << endl;
            cout << "\t\t---------------------------------------------" << endl;
            cout << endl;
            cout << "\tThe statistical analysis used in calculating the probabilities listed below " << endl;
            cout << "\tutilizes the binomial distribution. The binomial distribution is " << endl;
            cout << "\tused to model the number of successes in a fixed number of independent" << endl;
            cout << "\ttrials (a team's wins in a season). Hence, the data collected" << endl;
            cout << "\twill focus on the wins of each team in their respective leagues." << endl;
            cout << endl;
            cout << "\t\t---------------------------------------------" << endl;


            cout << endl;

            // show analysis menu
            teamMenu(teams, SIZE, user_choice);
            // user enters choice from the menu
            cout << "Enter your choice, it must be a number (eg. 1-7): ";
            cin >> stats_choice;

            // verify if use entered correct option (input validation)

            do
            {
                if (cin.fail() || stats_choice < 1 || stats_choice > 7){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Error! You must enter a number from (1-7): ";
                    cin >> stats_choice;
                }

            } while(cin.fail() || stats_choice < 1 || stats_choice > 7);

            // using switch statement to call the appropriate probability function
            switch(stats_choice)
            {
            case 1:
                {
                    cout << endl;
                    cout <<"---------------------------------" << endl;
                    cout << "1. Advancing the group stages" << endl;
                    cout <<"---------------------------------" << endl;
                    probabilities_groupStages_advancing(teams, SIZE, user_choice, stats_choice , position);

                    break;
                }

            case 2:
                {
                    cout << endl;
                    cout <<"----------------------------------------" << endl;
                    cout << "2. Elimination from the group stages" << endl;
                    cout <<"----------------------------------------" << endl;
                    probabilities_groupStages_elimination(teams, SIZE, user_choice, stats_choice , position);
                    break;
                }

            case 3:
                {
                    cout << endl;
                    cout <<"----------------------------------------" << endl;
                    cout << "3. Advancing past the knockout stages" << endl;
                    cout <<"----------------------------------------" << endl;
                    probabilities_knockouts_advancing(teams, SIZE, user_choice, stats_choice , position);
                    break;
                }

            case 4:
                {
                    cout << endl;
                    cout <<"-------------------------------------------" << endl;
                    cout << "4. Elimination from the knockout stages" << endl;
                    cout <<"-------------------------------------------" << endl;
                    probabilities_knockouts_elimination(teams, SIZE, user_choice, stats_choice , position);
                    break;
                }

            case 5:
                {
                    cout << endl;
                    cout <<"----------------------------------------" << endl;
                    cout << "5. Advancing past the semi-finals" << endl;
                    cout <<"----------------------------------------" << endl;
                    probabilities_semi_finals_advancing(teams, SIZE, user_choice, stats_choice , position);

                    break;
                }

            case 6:
                {
                    cout << endl;
                    cout <<"----------------------------------------" << endl;
                    cout << "6. Elimination from the semi-finals" << endl;
                    cout <<"----------------------------------------" << endl;
                    probabilities_semi_finals_elimination(teams, SIZE, user_choice, stats_choice , position);
                    break;
                }

            case 7:
                {
                    cout << endl;
                    cout << "-------------------" << endl;
                    cout << "7. The final" << endl;
                    cout << "-------------------" << endl;
                    string opponent;

                    cout << endl;
                    cout << "Enter the team that you would like " << user_choice << " to play with in the final."
                    << endl;
                    cout << "Your choice must be from the teams listed above." << endl;
                    cout << "\nTeam name of choice: ";

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, opponent);

                    // check if the user entered the right team name
                    bool found = false;
                    int position_second = -1;
                    int index = 0;

                    // Loop to search for the opponent team name
                    while (!found && index < SIZE)
                    {
                        if (teams[index].name == opponent){
                            found = true;
                            position_second = index;
                        }

                        index++;
                    }

                    if (found == false){
                        cout << "\nError! Club name not found."
                        << endl;
                    }

                    // If the opponent is found, calculate the final probability
                    else{
                        probabilities_finals(teams, SIZE, user_choice, stats_choice, position, opponent, position_second);
                    }

                    break;
                }
            }


        }

        // user makes a choice to either continue with the program or not
        do
        {
            cout << endl;
            cout << "Would you like to select another team? type (Y = yes or N = no): ";
            cin >> continue_program;
            continue_program = toupper(continue_program);

            if (continue_program != 'Y' && continue_program != 'N') {
                cout << "Error! You must either enter Y or N. Try again."
                << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

        } while (continue_program != 'Y' && continue_program != 'N');
        ////////////////////////

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // reset program


    } while (continue_program == 'Y');

    cout << endl;
    cout << "You are leaving the program. Good bye!" << endl;
    cout << endl;
    return 0;
}


// Probability Calculation Functions

// Function to calculate and display the probability of a team advancing past the group stages.
void probabilities_groupStages_advancing(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position)
{
    double probabilities[SIZE][8];    // Array to hold individual season win rates (wins/6 games)
    double group_stages[SIZE][1];     // Array to hold the average win rate across all 8 seasons
    double sum_prob = 0.0;            // Accumulator for the sum of win rates
    double p_value = 0.0;             // Stores the final binomial probability

    // Loop through all teams to calculate their average group stage win probability (p_value for binomial)
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            // Calculate the win rate for a single season (games won / 6 total games)
            probabilities[i][k] = static_cast<double>(teams[i].games_groupstage[0][k])/6.0;
            sum_prob += probabilities[i][k]; // Sum the win rates across all 8 seasons
        }

        // Calculate the average group stage win probability
        group_stages[i][0] = sum_prob/8.0;
        // Store the average probability back into the team's structure
        teams[i].group_stage_probabilities[i][0] = group_stages[i][0];

        sum_prob = 0.0; // Reset accumulator for the next team
    }

    // Calculate the final probability of advancing (winning k_min=4 out of n=6 games) using the binomial function
    p_value = binomial_function_groupstage(teams[position].group_stage_probabilities[position][0], 6);


    cout << endl;
    // Display the historical data used for the calculation
    cout << user_choice << " has the following historical group stage wins (W) from 2015/16 to 2022/23:" << endl;
    int year1 = 2015;
    int year2 = 16;

    for (int i = 0; i < 8; i++)
    {
        // Display wins per season
        cout << year1 << "/" << year2 << ": " << teams[position].games_groupstage[0][i] << " out of 6 group games (W)"<< endl;
        year1++;
        year2++;
    }

    cout << "----------------------------------" << endl;
    cout << endl;

    cout << "The seasons with the highest amount of wins: " << endl;

    // we find the max value and then display the max value with the corresponding season

    int highest_value = teams[position].games_groupstage[0][0];

    // we find the max value first
    for (int i = 0; i < 8; i++)
    {
        if (teams[position].games_groupstage[0][i] > highest_value){
            highest_value = teams[position].games_groupstage[0][i];
        }
    }

    for (int i = 0; i < 8; i++)
    {
        int yearOne = 2015;
        int yearTwo = 16;

        if (teams[position].games_groupstage[0][i] == highest_value){
            cout << yearOne + i << "/" << yearTwo + i << "| " << highest_value << " wins" << endl;
        }
    }



    cout << endl;

    // Display the final calculated probability, formatted as a percentage with no decimal places
    cout << "The probability of " << user_choice << " advancing the group is: "
        << setprecision(2) << fixed << p_value* 100 << "%" << endl;


}

// Function to calculate and display the probability of a team being eliminated from the group stages.
void probabilities_groupStages_elimination(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position)
{
    double probabilities[SIZE][8];
    double group_stages[SIZE][1];
    double sum_prob = 0.0;
    double p_value = 0.0; // Probability of advancing (P(A))

    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            probabilities[i][k] = static_cast<double>(teams[i].games_groupstage[0][k])/6.0;
            sum_prob += probabilities[i][k];
        }

        group_stages[i][0] = sum_prob/8.0;
        teams[i].group_stage_probabilities[i][0] = group_stages[i][0];

        sum_prob = 0.0;
    }

    // Calculate the probability of advancing P(A)
    p_value = binomial_function_groupstage(teams[position].group_stage_probabilities[position][0], 6);

    cout << endl;
    // Display historical losses (6 - wins) for context
    cout << user_choice << " has the following historical group stage losses (L) from 2015/16 to 2022/23:" << endl;
    int year1 = 2015;
    int year2 = 16;

    for (int i = 0; i < 8; i++)
    {
        // Calculate and display losses (6 total games - games won)
        cout << year1 << "/" << year2 << ": " << (6 - teams[position].games_groupstage[0][i]) << " out of 6 group games (L)"<< endl;
        year1++;
        year2++;
    }

    cout << "----------------------------------" << endl;
    cout << endl;

    cout << "The seasons with the highest amount of losses: " << endl;

    // we find the min value and then display the min value with the corresponding season

    int lowest_value = teams[position].games_groupstage[0][0];

    // we find the min value first
    for (int i = 0; i < 8; i++)
    {
        if (teams[position].games_groupstage[0][i] < lowest_value){
            lowest_value = teams[position].games_groupstage[0][i];
        }
    }

    for (int i = 0; i < 8; i++)
    {
        int yearOne = 2015;
        int yearTwo = 16;

        if (teams[position].games_groupstage[0][i] == lowest_value){
            int actual_losses = 6 - lowest_value;
            cout << yearOne + i << "/" << yearTwo + i << "| " << actual_losses << " losses" << endl;
        }
    }

    cout << endl;
    // The probability of elimination P(E) is 1 - P(A)
    cout << "The probability of " << user_choice << " getting eliminated from the group stages " << " is: "
        << setprecision(2) << fixed << (1 - p_value) * 100 << "%" << endl;
}


// Function to calculate and display the probability of a team advancing past the knockout stages (Round of 16, Quarter-finals, etc.).
void probabilities_knockouts_advancing(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position)
{
    double probabilities[SIZE][8];
    double knockouts_stages[SIZE][1];
    double sum_prob = 0.0;
    double p_value = 0.0;

    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            probabilities[i][k] = static_cast<double>(teams[i].games_knockouts[0][k])/2.0;
            sum_prob += probabilities[i][k]; // Sum the win rates
        }

        // Calculate the average knockout tie win probability across all 8 seasons
        knockouts_stages[i][0] = sum_prob/8.0;
        teams[i].knockouts_stage_probabilities[i][0] = knockouts_stages[i][0];

        sum_prob = 0.0; // Reset accumulator
    }

    // Calculate the final probability of advancing in a knockout tie (winning k_min=2 out of n=2 games)
    // In binomial_function_knockouts, winning the tie is defined as k_min=2 (winning both legs or winning on aggregate).
     p_value = binomial_function_knockouts(teams[position].knockouts_stage_probabilities[position][0], 2);

    // Display the historical data used for the calculation
    cout << user_choice << " has the following historical knockout stage wins (W) from 2015/16 to 2022/23:" << endl;
    int year1 = 2015;
    int year2 = 16;

    for (int i = 0; i < 8; i++)
    {
        cout << year1 << "/" << year2 << ": " << teams[position].games_knockouts[0][i] << " out of 2 knockout games (W)"<< endl;
        year1++;
        year2++;
    }

    cout << "----------------------------------" << endl;
    cout << endl;

    cout << "The seasons with the highest amount of wins: " << endl;

    // we find the max value and then display the max value with the corresponding season

    int highest_value = teams[position].games_knockouts[0][0];

    // we find the max value first
    for (int i = 0; i < 8; i++)
    {
        if (teams[position].games_knockouts[0][i] > highest_value){
            highest_value = teams[position].games_knockouts[0][i];
        }
    }

    for (int i = 0; i < 8; i++)
    {
        int yearOne = 2015;
        int yearTwo = 16;

        if (teams[position].games_knockouts[0][i] == highest_value){
            cout << yearOne + i << "/" << yearTwo + i << "| " << highest_value << " wins" << endl;
        }
    }
    cout << endl;
    cout << "The probability of " << user_choice << " advancing the knockout stages " << " is: "
        << setprecision(2) << fixed << p_value * 100 << "%" << endl;
}

// Function to calculate and display the probability of a team being eliminated from the knockout stages.
void probabilities_knockouts_elimination(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position)
{
    double probabilities[SIZE][8];
    double knockouts_stages[SIZE][1];
    double sum_prob = 0.0;
    double p_value = 0.0; // Probability of advancing P(A)

    // Calculate the average knockout tie win probability (P(win))
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            probabilities[i][k] = static_cast<double>(teams[i].games_knockouts[0][k])/2.0;
            sum_prob += probabilities[i][k];
        }

        knockouts_stages[i][0] = sum_prob/8.0;
        teams[i].knockouts_stage_probabilities[i][0] = knockouts_stages[i][0];

        sum_prob = 0.0;
    }

    // Calculate the probability of advancing P(A)
    p_value = binomial_function_knockouts(teams[position].knockouts_stage_probabilities[position][0], 2);

    cout << user_choice << " has the following historical knockout stage losses (L) from 2015/16 to 2022/23:" << endl;
    int year1 = 2015;
    int year2 = 16;

    for (int i = 0; i < 8; i++)
    {
        cout << year1 << "/" << year2 << ": " << (2 - teams[position].games_knockouts[0][i]) << " out of 2 knockout games (L)"<< endl;
        year1++;
        year2++;
    }
    cout << "----------------------------------" << endl;
    cout << endl;

    cout << "The seasons with the highest amount of losses: " << endl;

    // we find the max value and then display the max value with the corresponding season

    int lowest_value = teams[position].games_knockouts[0][0];

    // we find the max value first
    for (int i = 0; i < 8; i++)
    {
        if (teams[position].games_knockouts[0][i] < lowest_value){
            lowest_value = teams[position].games_knockouts[0][i];
        }
    }

    for (int i = 0; i < 8; i++)
    {
        int yearOne = 2015;
        int yearTwo = 16;

        if (teams[position].games_knockouts[0][i] == lowest_value){
            int actual_losses = 2 - lowest_value;
            cout << yearOne + i << "/" << yearTwo + i << "| " << actual_losses << " losses" << endl;
        }
    }

    cout << endl;
    // The probability of elimination P(E) is 1 - P(A)
    cout << "The probability of " << user_choice << " getting eliminated from the knockout stages " << " is: "
        << setprecision(2) << fixed << (1 - p_value) * 100 << "%" << endl;
}

// Function to calculate and display the probability of a team advancing past the semi-finals.
// This function uses historical domestic league wins as the probability of success (p_value).
void probabilities_semi_finals_advancing(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position)
{
    double probabilities[SIZE][8];
    double semi_finals[SIZE][1];
    double sum_prob = 0.0;
    double p_value = 0.0;

    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            // Calculate win rate for a season (league wins / 34 total league games)
            probabilities[i][k] = static_cast<double>(teams[i].games_semi_finals[0][k])/34.0;
            sum_prob += probabilities[i][k]; // Sum the win rates
        }

        // Calculate the average domestic league win probability across 4 seasons
        semi_finals[i][0] = sum_prob/4.0;
        teams[i].semi_finals_probabilities[i][0] = semi_finals[i][0];

        sum_prob = 0.0;
    }

    // Calculate the final probability of advancing in the semi-finals (winning k_min=2 out of n=2)
    p_value = binomial_function_semi_finals(teams[position].semi_finals_probabilities[position][0], 2);

    // Display the historical data (domestic league wins) used
    cout << user_choice << " has the following historical domestic league wins (W) from 2019/20 to 2022/23:" << endl;
    int year1 = 2019;
    int year2 = 20;

    for (int i = 0; i < 4; i++)
    {
        cout << year1 << "/" << year2 << ": " << (teams[position].games_semi_finals[0][i]) << " out of 34 " << teams[position].leagues<< " games (W)"<< endl;
        year1++;
        year2++;
    }

    cout << endl;
    cout << "----------------------------------" << endl;

    cout << "The seasons with the highest amount of wins: " << endl;

    // we find the max value and then display the max value with the corresponding season

    int highest_value = teams[position].games_semi_finals[0][0];

    // we find the max value first
    for (int i = 0; i < 4; i++)
    {
        if (teams[position].games_semi_finals[0][i] > highest_value){
            highest_value = teams[position].games_semi_finals[0][i];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        int yearOne = 2019;
        int yearTwo = 20;

        if (teams[position].games_semi_finals[0][i] == highest_value){
            cout << yearOne + i << "/" << yearTwo + i << "| " << highest_value << " wins" << endl;
        }
    }

    cout << endl;
    // Display the final calculated probability, formatted as a percentage
    cout << "The probability of " << user_choice << " advancing the semi-finals " << " is: "
        << setprecision(2) << fixed << (p_value) * 100 << "%" << endl;
    cout << endl;

    cout << "Note: The choice of using historical domestic league wins is due to the general positive " << endl;
    cout << "\tcorrelation between a team's historical domestic league titles and their likelihood of winning" << endl;
    cout << "\ta Champions league final, based on data of the last 21 finals (2005-2025), teams that reached the final with" << endl;
    cout << "\tmore league wins won 67% of the time. Additionally, as teams progress through to the latter stages of the" << endl;
    cout << "\ttournament, there is less data available to support predictions about their outcomes." << endl;
}

// Function to calculate and display the probability of a team being eliminated from the semi-finals.
void probabilities_semi_finals_elimination(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position)
{
    double probabilities[SIZE][8];
    double semi_finals[SIZE][1];
    double sum_prob = 0.0;
    double p_value = 0.0; // Probability of advancing P(A)

    // Calculate the average domestic league win probability (P(win))
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            probabilities[i][k] = static_cast<double>(teams[i].games_semi_finals[0][k])/34.0;
            sum_prob += probabilities[i][k];
        }

        semi_finals[i][0] = sum_prob/4.0;
        teams[i].semi_finals_probabilities[i][0] = semi_finals[i][0];

        sum_prob = 0.0;
    }

    // Calculate the probability of advancing P(A)
    p_value = binomial_function_semi_finals(teams[position].semi_finals_probabilities[position][0], 2);

    // Display historical losses (34 - wins) for context
    cout << user_choice << " has the following historical domestic league losses (L) from 2019/20 to 2022/23:" << endl;
    int year1 = 2019;
    int year2 = 20;

    for (int i = 0; i < 4; i++)
    {
        cout << year1 << "/" << year2 << ": " << (34 - teams[position].games_semi_finals[0][i]) << " out of 34 " << teams[position].leagues << " games (L)"<< endl;
        year1++;
        year2++;
    }

    cout << endl;
    cout << "----------------------------------" << endl;

    cout << "The seasons with the highest amount of losses: " << endl;

    // we find the min value and then display the min value with the corresponding season

    int lowest_value = teams[position].games_semi_finals[0][0];

    // we find the min value first
    for (int i = 0; i < 4; i++)
    {
        if (teams[position].games_semi_finals[0][i] < lowest_value){
            lowest_value = teams[position].games_semi_finals[0][i];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        int yearOne = 2019;
        int yearTwo = 20;

        if (teams[position].games_semi_finals[0][i] == lowest_value){
            cout << yearOne + i << "/" << yearTwo + i << "| " << 34 - lowest_value << " losses" << endl;
        }
    }
    cout << endl;
    // The probability of elimination P(E) is 1 - P(A)
    cout << "The probability of " << user_choice << " getting eliminated from the semi-finals " << " is: "
        << setprecision(2) << fixed << (1 - p_value) * 100 << "%" << endl;
    cout << endl;

    cout << "Note: The choice of using historical domestic league wins is due to the general positive " << endl;
    cout << "\tcorrelation between a team's historical domestic league titles and their likelihood of winning" << endl;
    cout << "\ta Champions league final, based on data of the last 21 finals (2005-2025), teams that reached the final with" << endl;
    cout << "\tmore league wins won 67% of the time. Additionally, as teams progress through to the latter stages of the" << endl;
    cout << "\ttournament, there is less data available to support predictions about their outcomes." << endl;
}

// Function to determine the predicted winner of the Champions League Final between two selected teams.
// The prediction is based on the comparison of their average historical domestic league win rates.
void probabilities_finals(Tournament teams[], int SIZE, string user_choice, int stats_choice, int position, string opponent, int position_second)
{
    double probabilities[SIZE][8];
    double finals[SIZE][1];
    double sum_prob = 0.0;
    double p_value_main = 0.0;    // Average league win probability for the user's team
    double p_value_second = 0.0;  // Average league win probability for the opponent

    // Calculate the average domestic league win probability (P(win)) for all teams
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            probabilities[i][k] = static_cast<double>(teams[i].games_finals[0][k])/34.0;
            sum_prob += probabilities[i][k];
        }

        finals[i][0] = sum_prob/4.0;
        teams[i].finals_probabilities[i][0] = finals[i][0];

        sum_prob = 0.0;
    }

    p_value_main = teams[position].finals_probabilities[position][0];
    p_value_second = teams[position_second].finals_probabilities[position_second][0];
    cout << endl;

    // Compare the probabilities to determine the predicted winner
    if (p_value_main > p_value_second){
        // User's team has a higher historical win rate
        cout << "The winner of the UEFA Champions league Final: " << user_choice << " (" << teams[position].leagues << ")" << endl;
        cout << user_choice << " has the probability of " << setprecision(2) << fixed << p_value_main * 100 << "%" << endl;
    }

    else if (p_value_main < p_value_second){
        // Opponent team has a higher historical win rate
        cout << "The winner of the UEFA Champions league Final: " << opponent << " (" << teams[position_second].leagues << ")" << endl;
        cout << opponent << " has the probability of " << setprecision(2) << fixed << p_value_second * 100 << "%" << endl;
    }

    else{
        // Probabilities are equal
        cout << "The game ends in a draw. Winner is decided on penalties" << endl;
    }

}



// Function to display the main welcome screen and the list of all 32 teams grouped into 8 groups (A-H).
void menu(Tournament teams[], int SIZE)
{
    cout << "\t\t Welcome to The UEFA Champions League statistics" << endl;
    cout << endl;
    cout << "This is the UEFA Champions league prediction calculator. The tournament fixtures 32 teams" << endl;
    cout << "that will battle it out from the group stages, knockouts, semi-finals and then the final." << endl;
    cout << endl;
    cout << "Select a team and find out about their chances in the tournament." << endl;
    cout << endl;
    cout << "Season: 2023-24" << endl;
    cout << "---------------" << endl;

    int group_size = 4;           // 4 teams per group
    int groups_per_row = 4;       // Display 4 groups per line (A-D, then E-H)
    int total_groups = 8;         // Total of 8 groups
    int team_index = 0;           // Index to access the 'teams' array
    int current_group = 0;        // Current group number (0-7)
    char group_name = ' ';        // Character for the group name

    for (int i = 0; i < total_groups; i += groups_per_row)
    {
        // Print group headers
        for (int j = 0; j < groups_per_row; j++) {
            current_group = i + j;
            if (current_group < total_groups) {
                group_name = 'A' + current_group; // Calculate group letter (A=0, B=1, etc.)
                cout << "Group " << group_name << ": " << setw(15) << left << "";
            }
        }
        cout << endl;

        // Print teams row by row (Team 1 of A, Team 1 of B, etc.)
        for (int k = 0; k < group_size; k++) { // k iterates through the 4 team slots in each group
            for (int j = 0; j < groups_per_row; j++) { // j iterates through the groups to be displayed in the current row
                current_group = i + j;
                if (current_group < total_groups) {
                    // Calculate the array index for the team in this group and position
                    team_index = current_group * group_size + k;
                    if (team_index < SIZE) {
                        // Display team number (1-4) and name, left-aligned
                        cout << k + 1 << ". " << setw(20) << left << teams[team_index].name;
                    }
                }
            }
            cout << endl; // Move to the next row of teams
        }
        cout << endl; // Space between the first 4 groups and the last 4 groups
    }
}

// Function to display the statistical analysis options menu for the selected team.
void teamMenu(Tournament teams[], int SIZE, string user_choice)
{

    cout << "Select " << user_choice << "'s statistical data" << endl;
    cout << endl;
    cout << "1. Advancing the group stages" << endl;
    cout << "2. Elimination from the group stages" << endl;
    cout << "3. Advancing past the knockout stages" << endl;
    cout << "4. Elimination from the knockout stages" << endl;
    cout << "5. Advancing past the semi-finals" << endl;
    cout << "6. Elimination from the semi-finals" << endl;
    cout << "7. The final" << endl;

    cout << endl;
}


// Calculates the binomial probability of achieving k_min=4 or more successes (wins) in n=6 trials (group games).
double binomial_function_groupstage(double p_value, int n)
{
    int k_min = 4;        // Minimum number of wins required to advance (4 wins out of 6 games)
    double total_prob = 0.0; // Accumulator for the total probability

    // Calculate n!
    double n_fact = 1.0;
    for (int i = 1; i <= n; i++)
    {
        n_fact *= i;
    }

    // Loop from k_min to n
    for (int kk = k_min; kk <= n; kk++)
    {
        // Calculate k! (kk factorial)
        double k_fact = 1.0;
        for (int j = 1; j <= kk; j++)
        {
            k_fact *= j;
        }

        // Calculate (n-k)! ((n-kk) factorial)
        double nk_fact = 1.0;
        for (int t = 1; t <= (n - kk); t++)
        {
            nk_fact *= t;
        }

        // Calculate the binomial coefficient
        double binom_coeff = n_fact / (k_fact * nk_fact);

        // Calculate the probability for exactly k successes: P(X=k) = C(n, k) * p^k * (1-p)^(n-k)
        double term_prob = binom_coeff * pow(p_value, kk) * pow(1.0 - p_value, n - kk);
        total_prob += term_prob; // Add to the total probability
    }

    return total_prob; // Return P(X >= 4)
}

// Calculates the binomial probability of advancing past a knockout tie (winning k_min=2 out of n=2 games).
// Success is defined as winning the tie, assumed to be winning both legs or winning on aggregate.
double binomial_function_knockouts(double p_value, int n)
{
    int k_min = 2; // Minimum number of wins (legs/games) required to win the tie (2 wins out of 2 games)
    double total_prob = 0.0;

    // Calculate n!
    double n_fact = 1.0;
    for (int i = 1; i <= n; i++)
    {
        n_fact *= i;
    }

    // Loop from k_min to n
    for (int kk = k_min; kk <= n; kk++)
    {
        // Calculate k!
        double k_fact = 1.0;
        for (int j = 1; j <= kk; j++)
        {
            k_fact *= j;
        }

        // Calculate (n-k)!
        double nk_fact = 1.0;
        for (int t = 1; t <= (n - kk); t++)
        {
            nk_fact *= t;
        }

        // Calculate the binomial coefficient
        double binom_coeff = n_fact / (k_fact * nk_fact);

        // Calculate the probability for exactly k successes
        double term_prob = binom_coeff * pow(p_value, kk) * pow(1.0 - p_value, n - kk);
        total_prob += term_prob;
    }

    return total_prob; // Return P(X = 2)
}

// Calculates the binomial probability of advancing past a semi-final tie (winning k_min=2 out of n=2 games).
// The logic is the same as the knockout function, but it uses the league win rate as its p_value.
double binomial_function_semi_finals(double p_value, int n)
{
    int k_min = 2; // Minimum number of wins (legs/games) required to win the tie (2 wins out of 2 games)
    double total_prob = 0.0;

    // Calculate n!
    double n_fact = 1.0;
    for (int i = 1; i <= n; i++)
    {
        n_fact *= i;
    }

    // Loop from k_min to n (i.e., calculate P(X=2) only)
    for (int kk = k_min; kk <= n; kk++)
    {
        // Calculate k!
        double k_fact = 1.0;
        for (int j = 1; j <= kk; j++)
        {
            k_fact *= j;
        }

        // Calculate (n-k)!
        double nk_fact = 1.0;
        for (int t = 1; t <= (n - kk); t++)
        {
            nk_fact *= t;
        }

        // Calculate the binomial coefficient
        double binom_coeff = n_fact / (k_fact * nk_fact);

        // Calculate the probability for exactly k successes
        double term_prob = binom_coeff * pow(p_value, kk) * pow(1.0 - p_value, n - kk);
        total_prob += term_prob;
    }

    return total_prob; // Return P(X = 2)
}
