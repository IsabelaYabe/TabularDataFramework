#ifndef TIME_H
#define TIME_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>

class Time {
private:
  int day;
  int month;
  int year;
  int hour;
  int minute;
  int second;
 public:
 enum class TimeMeasure { Year, Month, Day, Hour, Minute, Second }; 
 enum class CompareType { Less, LessOrEqual, Equal, Greater, GreaterOrEqual };

  // Construtor com parâmetros para inicializar diretamente os atributos.
  Time(int day, int month, int year, int hour=0, int minute=0, int second=0)
      : day(day), month(month), year(year), hour(hour), minute(minute), second(second) {}

  static std::optional<Time> fromString(const std::string& dateTimeString) {
    std::istringstream ss(dateTimeString);
    int d, m, y, hr, min, sec;
    char dash1, dash2, colon1, colon2, space;

    ss >> m >> dash1 >> d >> dash2 >> y >> space >> hr >> colon1 >> min >> colon2 >> sec;
    if (ss.fail() || dash1 != '-' || dash2 != '-' || colon1 != ':' || colon2 != ':' || space != ' ') {
        return std::nullopt; // Return an empty optional if parsing fails
    }

    return Time(d, m, y, hr, min, sec);
  }

  // Métodos de acesso (getters) para os atributos.
  int getDay() const { return day; }
  int getMonth() const { return month; }
  int getYear() const { return year; }
  int getHour() const { return hour; }
  int getMinute() const { return minute; }
  int getSecond() const { return second; }

  // Métodos de modificação (setters) para os atributos.
  void setDay(int newDay) { day = newDay; }
  void setMonth(int newMonth) { month = newMonth; }
  void setYear(int newYear) { year = newYear; }
  void setHour(int newHour) { hour = newHour; }
  void setMinute(int newMinute) { minute = newMinute; }
  void setSecond(int newSecond) { second = newSecond; }

  // Método para imprimir a data e hora formatada.
  void print() const {
    std::cout << std::setfill('0') << std::setw(2) << day << "-"
              << std::setfill('0') << std::setw(2) << month << "-"
              << year << " "
              << std::setfill('0') << std::setw(2) << hour << ":"
              << std::setfill('0') << std::setw(2) << minute << ":"
              << std::setfill('0') << std::setw(2) << second << std::endl;
  }
static bool compareTime(const Time& a, const Time& b, TimeMeasure measure, CompareType cmpType) {
    auto compare = [&](int lhs, int rhs) -> bool {
        switch (cmpType) {
            case CompareType::Less: return lhs < rhs;
            case CompareType::LessOrEqual: return lhs <= rhs;
            case CompareType::Equal: return lhs == rhs;
            case CompareType::Greater: return lhs > rhs;
            case CompareType::GreaterOrEqual: return lhs >= rhs;
            default: throw std::invalid_argument("Invalid comparison type.");
        }
    };

    if (measure >= TimeMeasure::Year && a.getYear() != b.getYear())
        return compare(a.getYear(), b.getYear());
    if (measure >= TimeMeasure::Month && a.getMonth() != b.getMonth())
        return compare(a.getMonth(), b.getMonth());
    if (measure >= TimeMeasure::Day && a.getDay() != b.getDay())
        return compare(a.getDay(), b.getDay());
    if (measure >= TimeMeasure::Hour && a.getHour() != b.getHour())
        return compare(a.getHour(), b.getHour());
    if (measure >= TimeMeasure::Minute && a.getMinute() != b.getMinute())
        return compare(a.getMinute(), b.getMinute());
    if (measure >= TimeMeasure::Second && a.getSecond() != b.getSecond())
        return compare(a.getSecond(), b.getSecond());
    
    return cmpType == CompareType::Equal || cmpType == CompareType::LessOrEqual || cmpType == CompareType::GreaterOrEqual;
  }
  
    friend bool operator!=(const Time& lhs, const Time& rhs) {
        return !(lhs.getSecond() == rhs.getSecond() &&
                 lhs.getMinute() == rhs.getMinute() &&
                 lhs.getHour() == rhs.getHour() &&
                 lhs.getDay() == rhs.getDay() &&
                 lhs.getMonth() == rhs.getMonth() &&
                 lhs.getYear() == rhs.getYear());
    }

    friend bool operator==(const Time& lhs, const Time& rhs) {
        return lhs.getSecond() == rhs.getSecond() &&
               lhs.getMinute() == rhs.getMinute() &&
               lhs.getHour() == rhs.getHour() &&
               lhs.getDay() == rhs.getDay() &&
               lhs.getMonth() == rhs.getMonth() &&
               lhs.getYear() == rhs.getYear();
    }

    friend bool operator<(const Time& lhs, const Time& rhs) {
        if (lhs.getYear() != rhs.getYear()) return lhs.getYear() < rhs.getYear();
        if (lhs.getMonth() != rhs.getMonth()) return lhs.getMonth() < rhs.getMonth();
        if (lhs.getDay() != rhs.getDay()) return lhs.getDay() < rhs.getDay();
        if (lhs.getHour() != rhs.getHour()) return lhs.getHour() < rhs.getHour();
        if (lhs.getMinute() != rhs.getMinute()) return lhs.getMinute() < rhs.getMinute();
        return lhs.getSecond() < rhs.getSecond();
    }

    friend bool operator>(const Time& lhs, const Time& rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const Time& lhs, const Time& rhs) {
        return !(lhs > rhs);
    }

    friend bool operator>=(const Time& lhs, const Time& rhs) {
        return !(lhs < rhs);
    }

};


#endif // TIME_H