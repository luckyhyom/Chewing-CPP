#include <iostream>

class Date {
    int year_;
    int month_;
    int day_;

    public:
        void AddDay(int inc);
        void AddMonth(int inc);
        void AddYear(int inc);
        void ShowDate();
        Date() {
            year_ = 2024;
            month_ = 1;
            day_ = 31;
        }
        Date(int year, int month, int day) { // 오버로딩 기능이 같은 함수명을 재사용할수있게 해준다.
            year_ = year;
            month_ = month;
            day_ = day;
        }

    private:
        void IncreaseYear() {
            year_ += 1;
            checkMonth();
        }

        void IncreaseMonth() {
            month_ += 1;
            if(month_ > 12) {
                year_ +=1;
                month_ = 1;
            }
            checkMonth();
        }

        void IncreaseDay() {
            day_ += 1;
            checkMonth();
        }

        void handleForFeb() {
            if(year_ % 4 == 0) {
                if(day_ > 29) {
                    day_ -= 29;
                    IncreaseMonth();
                }
            } else {
                if(day_ > 28) {
                    day_ -= 28;
                    IncreaseMonth();
                }
            }
        }

        void checkMonth() {
            if(month_ == 2) {
                handleForFeb();
            } else if (month_ == 4 || month_ == 6 || month_ == 9 || month_ == 11) {
                if(day_ > 30) {
                    day_ -= 30;
                    IncreaseMonth();
                }
            } else {
                if(day_ > 31) {
                    day_ -= 31;
                    IncreaseMonth();
                }
            }
        }
};

void Date:: AddDay(int inc) {
    for (int i = 0; i < inc; i++) IncreaseDay();
};

void Date:: AddMonth(int inc) {
    for (int i = 0; i < inc; i++) IncreaseMonth();
};

void Date:: AddYear(int inc) {
    for (int i = 0; i < inc; i++) IncreaseYear();
};

void Date:: ShowDate() {
    std::cout << year_ << "/" << month_ << "/" << day_ << std::endl;
}


int main() {
    Date date = Date(2024,1,31);
    date.ShowDate();
    for (int i = 0; i < 365; i++)
    {
        date.AddDay(1);
        date.ShowDate();
    }
    
    return 0;
}