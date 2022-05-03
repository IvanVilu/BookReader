#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>

using namespace std;

class BookReader {
public:
    
    //  Вывести в cout ответ на запрос  CHEER
    void PrintCheer(int user_id) {
        
        optional<int> cheer = GetBadCheer(user_id);
        if(cheer) {
            cout << cheer.value();
        } else {
            int user_read = user_to_page_[user_id];
            auto last = users_progress_.begin();
            advance(last, user_read);
            
            // Суммируем количество пользователей, прочитавших меньшее кол-во страниц
            int less_read_users_count = accumulate(users_progress_.begin(), last, 0);

            if(less_read_users_count == 0) {
               // Если текущий пользователь единственный
               if(users_count_ == 1)
                   cout << 1;
                else
                   cout << 0;
                   
            } else {
                // Считаем отношение, минус один текущий пользователь
                cout << static_cast<double>(less_read_users_count) / static_cast<double>(users_count_ - 1);
            }
        }
}
    
    void Read(int user_id, int page) {
        
        // Увеличить вектор если ID выходит за его педелы
        if(user_to_page_.size() <= user_id) {
            user_to_page_.resize(user_id + 1);
        }
        
        // Если для пользователя уже был вызван Read - очищаем эту информацию
        // иначе - это первый вызов Read - добавляем пользователя в счетчик
        if(user_to_page_[user_id] != 0)
            --users_progress_[user_to_page_[user_id]];
        else
            ++users_count_;
        
        user_to_page_[user_id] = page;
        
        // Если текущая страница пользователя выходит за пределы вектора страниц
        if(users_progress_.size() <= page)
            users_progress_.resize(page + 1);
        
        ++users_progress_[page];
    }
    
private:
    vector<int> user_to_page_; // ID пользователя(индекс вектора) к количеству страниц
    vector<int> users_progress_; // страница(индекс вектора) к количеству пользователей
    int users_count_ = 0; // Счетчик активных пользователей (для которых уже вызывался Read)
    
    // Проверка Cheer для пользователей, для которых не было вызвано READ или CHEER
    optional<int> GetBadCheer(int user_id) {
        
        // Если ID пользователя выходит за пределы существующих
        if(user_to_page_.size() <= user_id) {
            user_to_page_.resize(user_id+1);
            return 0;
        }
        // Если пользователь не прочел ни одной страницы
        if(user_to_page_[user_id] == 0) {
            return 0;
        }
        
        //  Пустое значение если всё ОК
        return nullopt;
    }
};

int main() {
    
    BookReader reader;
    int query_count;
    cin >> query_count;
    
    for(int i = 0; i < query_count; ++i) {
        string query;
        int id;
        cin >> query >> id;
        if(query == "CHEER") {
            reader.PrintCheer(id);
            
            // Выводим разрыв строки, если это не последняя итерация цикла
            if(i != query_count - 1) {
                cout << endl;
            }
            
        } else if(query == "READ") {
            int page;
            cin >> page;
            reader.Read(id, page);
        }
        
    }
    return 0;
}
