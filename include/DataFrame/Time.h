#ifndef TIME_H
#define TIME_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>

using namespace std;
/**
 * @class Time
 * @brief Class to represent and manipulate dates and times.
 *
 * This class allows creating and manipulating date and time objects, providing
 * methods for comparison, formatted input and output. It supports common operations
 * such as time comparison and converting strings to Time objects.
 */
class Time {
private:
    int day;    ///< Day of the month [1-31]
    int month;  ///< Month of the year [1-12]
    int year;   ///< Year
    int hour;   ///< Hour of the day [0-23]
    int minute; ///< Minute of the hour [0-59]
    int second; ///< Second of the minute [0-59]

 public:
    enum class TimeMeasure { Year, Month, Day, Hour, Minute, Second };  ///< Enum to specify time unit for comparison
    enum class CompareType { Less, LessOrEqual, Equal, Greater, GreaterOrEqual }; ///< Enum to specify type of comparison

    /**
    * @brief Constructor that initializes a Time object with specific date and time.
    * @param day Day of the month
    * @param month Month
    * @param year Year
    * @param hour Hour (default 0)
    * @param minute Minute (default 0)
    * @param second Second (default 0)
    */
    Time(int day, int month, int year, int hour=0, int minute=0, int second=0)
        : day(day), month(month), year(year), hour(hour), minute(minute), second(second) {} 
    /**
     * @brief Creates a Time object from a formatted string.
     * @param dateTimeString String containing the date and time in the format "YYYY-MM-DD HH:MM:SS"
     * @return optional<Time> A Time object if successful, nullopt otherwise.
     */
    static optional<Time> fromString(const std::string& dateTimeString) {
        // Validar o tamanho mínimo da string
        if (dateTimeString.size() < 19) return std::nullopt;

        // Extração correta dos componentes da data e hora
        std::string Y = dateTimeString.substr(0, 4);
        std::string M = dateTimeString.substr(5, 2);
        std::string D = dateTimeString.substr(8, 2);
        std::string HR = dateTimeString.substr(11, 2);
        std::string MIN = dateTimeString.substr(14, 2);
        std::string SEC = dateTimeString.substr(17, 2);

        try {
            int y = std::stoi(Y);
            int m = std::stoi(M);
            int d = std::stoi(D);
            int hr = std::stoi(HR);
            int min = std::stoi(MIN);
            int sec = std::stoi(SEC);

            // Cria e retorna um objeto Time se todos os valores forem convertidos corretamente
            return Time(d, m, y, hr, min, sec);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Erro: String inválida - " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Erro: Número fora do intervalo de int - " << e.what() << std::endl;
        }

        // Retorna nullopt se ocorrer qualquer erro
        return std::nullopt;
    }

    // Getters
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    int getHour() const { return hour; }
    int getMinute() const { return minute; }
    int getSecond() const { return second; }

    // Setters
    void setDay(int newDay) { day = newDay; }
    void setMonth(int newMonth) { month = newMonth; }
    void setYear(int newYear) { year = newYear; }
    void setHour(int newHour) { hour = newHour; }
    void setMinute(int newMinute) { minute = newMinute; }
    void setSecond(int newSecond) { second = newSecond; }   
    /**
     * @brief Prints the date and time in the format "DD-MM-YYYY HH:MM:SS".
     */
    void print() const {
      cout << setfill('0') << setw(2) << day << "-"
                << setfill('0') << setw(2) << month << "-"
                << year << " "
                << setfill('0') << setw(2) << hour << ":"
                << setfill('0') << setw(2) << minute << ":"
                << setfill('0') << setw(2) << second << endl;
    }
     /**
    * @brief Compares two Time instances according to a specified time unit and comparison type.
    * @param a First Time instance for comparison.
    * @param b Second Time instance for comparison.
    * @param measure Measurement unit for comparison (e.g., Year, Month, Day).
    * @param cmpType Type of comparison (e.g., Less, Greater).
    * @return bool Result of the comparison as specified by the type.
    */
    static bool compareTime(const Time& a, const Time& b, TimeMeasure measure, CompareType cmpType) {
        auto compare = [&](int lhs, int rhs) -> bool {
            switch (cmpType) {
                case CompareType::Less: return lhs < rhs;
                case CompareType::LessOrEqual: return lhs <= rhs;
                case CompareType::Equal: return lhs == rhs;
                case CompareType::Greater: return lhs > rhs;
                case CompareType::GreaterOrEqual: return lhs >= rhs;
                default: throw invalid_argument("Invalid comparison type.");
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
  
    // Comparison operators
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