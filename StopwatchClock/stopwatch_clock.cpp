#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <limits>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Global history logs
vector<string> history;

// ASCII bell beep
void beep() {
    cout << "\a";
    cout.flush();
}

void displayTime(milliseconds ms) {
    int sec = ms.count() / 1000;
    int ms_part = ms.count() % 1000;
    cout << sec << " sec, " << ms_part << " ms\n";
}

void stopwatchSession() {
    bool running = true;
    bool paused = false;
    milliseconds totalDuration(0);
    vector<milliseconds> lapTimes;

    auto start = high_resolution_clock::now();
    auto lapStart = start;

    cout << "\n✅ Stopwatch started.\n";

    while (running) {
        cout << "\n--- Stopwatch Controls ---\n";
        cout << "1. Pause\n";
        cout << "2. Resume\n";
        cout << "3. Lap\n";
        cout << "4. Stop\n";
        cout << "Enter your choice: ";

        int subChoice;
        cin >> subChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "❌ Invalid input.\n";
            continue;
        }

        switch (subChoice) {
            case 1:
                if (!paused) {
                    auto now = high_resolution_clock::now();
                    totalDuration += duration_cast<milliseconds>(now - start);
                    paused = true;
                    cout << "⏸️ Stopwatch paused.\n";
                } else {
                    cout << "Already paused.\n";
                }
                break;

            case 2:
                if (paused) {
                    start = high_resolution_clock::now();
                    lapStart = start;
                    paused = false;
                    cout << "▶️ Stopwatch resumed.\n";
                } else {
                    cout << "Stopwatch is already running.\n";
                }
                break;

            case 3: {
                if (!paused) {
                    auto now = high_resolution_clock::now();
                    milliseconds lap = duration_cast<milliseconds>(now - lapStart);
                    lapTimes.push_back(lap);
                    lapStart = now;
                    cout << "🏁 Lap " << lapTimes.size() << " recorded: ";
                    displayTime(lap);
                    beep(); // Lap beep
                } else {
                    cout << "Can't record lap while paused.\n";
                }
                break;
            }

            case 4: {
                if (!paused) {
                    auto now = high_resolution_clock::now();
                    totalDuration += duration_cast<milliseconds>(now - start);
                }

                cout << "\n🛑 Stopwatch stopped.\n";
                cout << "✅ Total time: ";
                displayTime(totalDuration);
                beep(); // Final beep

                string record = "Stopwatch Session - Total: " + to_string(totalDuration.count() / 1000.0) + " sec";
                history.push_back(record);

                if (!lapTimes.empty()) {
                    for (size_t i = 0; i < lapTimes.size(); ++i) {
                        string lapRecord = "Lap " + to_string(i + 1) + ": " + to_string(lapTimes[i].count() / 1000.0) + " sec";
                        history.push_back(lapRecord);
                    }
                }

                running = false;
                break;
            }

            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}

void countdownTimer() {
    int durationSec;
    cout << "\n⏳ Enter countdown time in seconds: ";
    cin >> durationSec;

    if (cin.fail() || durationSec <= 0) {
        cout << "❌ Invalid time.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Countdown started!\n";

    for (int i = durationSec; i > 0; --i) {
        cout << "⏲️  " << i << "...\n";
        this_thread::sleep_for(seconds(1));
    }

    cout << "\n🔔 Time's up!\n";
    beep();

    history.push_back("Countdown Timer - " + to_string(durationSec) + " seconds completed.");
}

void showCurrentTime() {
    auto now = system_clock::to_time_t(system_clock::now());
    cout << "🕰️ Current Time: " << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << endl;
    history.push_back("Checked current time.");
}

void quickStopwatch() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer

    cout << "🟢 Quick Stopwatch started. Press Enter to stop..." << endl;

    auto start = high_resolution_clock::now(); // Start timing before wait
    cin.get();  // Wait for Enter
    auto stop = high_resolution_clock::now();  // Stop timing

    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Elapsed time: ";
    displayTime(duration);

    string record = "Quick Stopwatch: " + to_string(duration.count() / 1000.0) + " sec";
    history.push_back(record);
}

void showHistory() {
    cout << "\n📜 --- History Log ---\n";
    if (history.empty()) {
        cout << "No entries.\n";
    } else {
        for (const auto& entry : history)
            cout << "- " << entry << endl;
    }
}

int main() {
    while (true) {
        cout << "\n============================\n";
        cout << "       🕒 Timer Tool Menu       \n";
        cout << "============================\n";
        cout << "1. Start Interactive Stopwatch (Lap/Pause/Resume)\n";
        cout << "2. Start Quick Stopwatch (Press Enter to stop)\n";
        cout << "3. Countdown Timer\n";
        cout << "4. Show Current Time\n";
        cout << "5. View History Log\n";
        cout << "0. Quit\n";
        cout << "Enter your choice: ";

        int mainChoice;
        cin >> mainChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "❌ Invalid input.\n";
            continue;
        }

        switch (mainChoice) {
            case 1:
                stopwatchSession();
                break;
            case 2:
                quickStopwatch();
                break;
            case 3:
                countdownTimer();
                break;
            case 4:
                showCurrentTime();
                break;
            case 5:
                showHistory();
                break;
            case 0:
                cout << "👋 Exiting... Goodbye!\n";
                return 0;
            default:
                cout << "❌ Invalid choice. Try again.\n";
        }
    }
    return 0;
}
