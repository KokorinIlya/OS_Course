//
// Created by ilya on 13.05.18.
//

#include <vector>
#include <iostream>
#include "fill_db.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

static vector<pair<string, string>> eng_rus = { {"come", "приходить"}, {"get", "получать"}, {"give", "давать"}, {"go", "ходить"}, {"keep", "продолжать"}, {"let", "позволять"}, {"make", "делать"}, {"put", "помещать"}, {"seem", "казаться"}, {"take", "брать"}, {"be", "быть"}, {"do", "делать"}, {"have", "иметь"}, {"say", "говорить"}, {"see", "видеть"}, {"send", "посылать"}, {"may", "мочь"}, {"will", "быть хотеть"}, {"about", "о"}, {"across", "через"}, {"after", "после"}, {"against", "против"}, {"among", "среди"}, {"at", "в"}, {"before", "перед"}, {"between", "между"}, {"by", "к"}, {"down", "вниз"}, {"from", "из"}, {"in", "в"}, {"off", "прочь"}, {"on", "на"}, {"over", "по"}, {"through", "через"}, {"to", "к"}, {"under", "под"}, {"up", "вверх"}, {"with", "с"}, {"as", "поскольку"}, {"for", "для"}, {"of", "из"}, {"till", "пока"}, {"than", "чем"}, {"a", "любой"}, {"all", "все"}, {"any", "любой"}, {"every", "каждый"}, {"no", "никакой"}, {"other", "другой"}, {"some", "некоторый"}, {"such", "такой"}, {"that", "что"}, {"this", "это"}, {"i", "я"}, {"he", "он"}, {"you", "ты"}, {"who", "кто"}, {"and", "и"}, {"because", "потому что"}, {"but", "а"}, {"or", "или"}, {"if", "если"}, {"though", "хотя"}, {"while", "в то время как"}, {"how", "как"}, {"when", "когда"}, {"where", "где"}, {"again", "снова"}, {"ever", "когда-либо"}, {"far", "самый дальний"}, {"forward", "отправлять"}, {"here", "здесь"}, {"near", "рядом"}, {"now", "теперь"}, {"out", "вне"}, {"still", "все еще"}, {"then", "тогда"}, {"there", "там"}, {"together", "вместе"}, {"well", "хорошо"}, {"almost", "почти"}, {"enough", "достаточно"}, {"even", "еще"}, {"little", "маленький"}, {"much", "много"}, {"not", "не"}, {"only", "только"}, {"quite", "весьма"}, {"so", "так"}, {"very", "очень"}, {"tomorrow", "завтра"}, {"yesterday", "вчера"}, {"north", "север"}, {"south", "юг"}, {"east", "восток"}, {"west", "запад"}, {"please", "пожалуйста"}, {"yes", "да"} };

void fill_databases(map<string, map<string, string>>& dicts)
{
    dicts["eng->rus"] = map<string, string>();
    dicts["rus->eng"] = map<string, string>();
    for (auto const& p: eng_rus)
    {
        dicts["eng->rus"][p.first] = p.second;
        dicts["rus->eng"][p.second] = p.first;
    }
}