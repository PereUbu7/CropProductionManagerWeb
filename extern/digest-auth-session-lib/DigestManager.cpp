/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Bertrand Martel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/**
    DigestManager.cpp
    Client socket managing event handler

    @author Bertrand Martel
    @version 1.0
*/
#include "DigestManager.h"
#include "Digestinfo.h"

#include "iostream"
#include <iomanip>
#include "sstream"
#include <string>
// #include "protocol/inter/http/IhttpFrame.h"
#include "utils/timeUtil.h"
#include "utils/stringutil.h"
// #include "QCryptographicHash"
#include <openssl/sha.h>

std::string Sha1(const std::string& str)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha;
    SHA1_Init(&sha);
    SHA1_Update(&sha, str.c_str(), str.size());
    SHA1_Final(hash, &sha);
    std::stringstream ss;
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string Sha256(const std::string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string Md5(const std::string& data)
{
    return data;
}

/**
 * @brief DigestManager::DigestManager
 *      Build digest manager
 */
DigestManager::DigestManager(){
    //seed for random generated string
    srand(time(0));

    //algorithm is sent to client by default (RFC2617 compliant)
    show_algorithm=true;

    //default algorithm is MD5
    algorithm=ALGO_SHA256;

    //nonce timeout default is 10 minutes
    nonce_timeout_millis=10*60*1000;

    session_t=SESSION_DIGEST;

    session_time_seconds=3600;
}

/**
 * @brief DigestManager::remove_expired_nonces
 *      Remove all nonces in map that expired
 */
void DigestManager::remove_expired_nonces(){
}

/**
 * @brief DigestManager::remove_expired_sessions
 *      Remove all session in map that expired
 */
void DigestManager::remove_expired_sessions(){
}


/**
 * @brief DigestManager::set_session_type
 *      Set session type (default is DIGEST session)
 * @param session
 */
void DigestManager::set_session_type(session_type session){
    this->session_t=session;
}

/**
 * @brief DigestManager::get_session_type
 *      Retrieve session type
 * @return
 */
session_type DigestManager::get_session_type(){
    return session_t;
}

/**
 * @brief DigestManager::set_nonce_timeout_millis
 *      set nonce timeout in milliseconds
 *  @param nonce_timeout
 */
void DigestManager::set_nonce_timeout_millis(long nonce_timeout){
    nonce_timeout_millis=nonce_timeout;
}

/**
 * @brief DigestManager::get_nonce_timeout_millis
 *      Retrieve nonce timeout in milliseconds
 * @return
 */
long DigestManager::get_nonce_timeout_millis(){
    return nonce_timeout_millis;
}

/**
 * @brief DigestManager::set_digest_algorithm
 *      Set digest algorithm used for processing hash
 * @param algorithm
 */
void DigestManager::set_digest_algorithm(digest_algorithm algorithm) {
    this->algorithm=algorithm;
}

/**
 * @brief DigestManager::get_algorithm
 *      Retrieve digest algorithm used for processing hash
 * @return
 */
digest_algorithm DigestManager::get_algorithm(){
    return algorithm;
}

/**
 * @brief DigestManager::set_max_session_time
 *      Set maximum session time for user
 * @param session_time
 */
void DigestManager::set_max_session_time(int session_time){
    this->session_time_seconds=session_time;
}

/**
 * @brief DigestManager::get_max_session_time
 *      Retrieve maximum session time for user
 * @return
 */
int DigestManager::get_max_session_time(){
    return session_time_seconds;
}

/**
 * @brief remove_session_for_headers
 *      Remove a session (cookie or digest)
 * @param headers
 * @return
 */
int DigestManager::remove_session_for_headers(std::map<std::string,std::string> *headers){

    std::cout << "remove session id\n";

    std::map<std::string,std::string> map_val=*headers;

    if (session_t==SESSION_COOKIE){
        std::string currentHsid="";

        if (map_val.find("Cookie")!=map_val.end()){
            currentHsid = getAuthenticationCookieField(headers->at("Cookie"));
        }

        if (currentHsid != ""){

            if (session_map.find(currentHsid)!=session_map.end()) {
                session_map.erase(currentHsid);
                std::cout << "HSID successfully removed\n";
                return 200;
            }
            else{
                std::cout << "HDSID header not registered\n";
            }
        }
        else{
            std::cout << "no HDSID header received\n";
        }
    } else {

        if (map_val.find("Authorization")!=map_val.end()){

            std::string authorizationHeader=headers->at("Authorization");

            std::map<std::string, std::string> subheaderList = splitHeader(authorizationHeader,',');

            if (subheaderList.find("opaque")==subheaderList.end()){
                std::cerr<< "No opaque subheader\n";
                return 500;
            }

            std::string opaque = subheaderList["opaque"];

            //check if peer address has an entry in map
            if (nonce_map.find(opaque)==nonce_map.end()){
                nonce_map.erase(opaque);
                std::cout << "Opaque successfully removed\n";
                return 200;
            }else{
                std::cerr << "Opaque not found in list\n";
            }
        }else{
            std::cerr << "No authentication headers\n";
        }
    }

    return 500;
}

/**
 * called when an http response has been received from client
 *
 * @param client
 * 		client object
 * @param message
 * 		message delivered
 */
DigestInfo DigestManager::process_digest(std::string method,std::string uri,std::map<std::string,std::string> headers,std::string realm){

    //############################################################################################

    bool authenticated = false;

    if (session_t==SESSION_COOKIE){
        std::string currentHsid="";

        if (headers.find("Cookie")!=headers.end()){
            currentHsid = getAuthenticationCookieField(headers.at("Cookie"));
        }

        if (currentHsid != ""){

            auto now = getTimeSinceEpoch<std::chrono::milliseconds>();

            if (session_map.find(currentHsid)!=session_map.end() && session_map[currentHsid].validity) {
                if (((now - session_map[currentHsid].session_start_time)<(session_time_seconds*1000))){
                     authenticated=true;
                 }
                else{
                    std::cout << "session id " << currentHsid << " expired\n";
                    session_map.erase(currentHsid);
                }
            }
        }
        else{
            std::cout << "no HDSID header received\n";
        }
    }

    //proces digest response if Authroization and Cookie header are present
    if (!authenticated && headers.find("Authorization")!=headers.end()){

        std::string authentication=headers.at("Authorization");
        std::string cookie="";

        if (headers.find("Cookie")!=headers.end()){
            cookie=headers.at("Cookie");
        }

        std::cout << "[DIGEST MANAGER] PROCESS RESPONSE\n";
        return processDigestResponse(authentication,cookie,realm,"GET",uri);
    }

    //process digest handshake if not authenticated
    if (!authenticated){

        if (headers.count("Host")>0){

            std::string host=headers.at("Host");

            std::cout << "[DIGEST MANAGER] PROCESS REQUEST\n";

            return generateHandshakeProcess(host,realm);
        }
        else{

            std::cout  << "ERROR : No host header found in request\n";

            std::multimap<std::string, std::string> headers_ret;
            DigestInfo digest_response(500,headers_ret);

            return digest_response;
        }
    }

    if (authenticated){

        std::multimap<std::string, std::string> headers_ret;
        DigestInfo digest_response(200,headers_ret);
        return digest_response;
    }


    std::multimap<std::string, std::string> headers_ret;
    DigestInfo digest_response(401,headers_ret);
    return digest_response;
}

/**
 * @brief DigestManager::getAuthenticationCookieField
 *  parse content of cookie header to retrieve value of HSID value :
 *
 *  for input : "HSID=AYQEVn….DKrdst; Domain=.foo.com; Path=/; Expires=Wed, 13 Jan 2021 22:23:01 GMT; HttpOnly"
 *  will return : "AYQEVn….DKrdst"
 *
 * @param cookieStr
 * @return
 */
std::string DigestManager::getAuthenticationCookieField(std::string cookieStr){

    if (cookieStr != ""){

        std::map<std::string, std::string> cookiesList = splitHeader(cookieStr,';');
        std::map< std::string, std::string >::iterator it = cookiesList.find("HSID");

        if(it != cookiesList.end()){

            return it->second;
        }
    }
    return "";
}

/**
 * @brief DigestManager::splitHeader
 *      Split header delimited with "delim" character
 * @param header
 *      input http header
 * @param delim
 *      character delimiter
 * @return
 */
std::map<std::string,std::string> DigestManager::splitHeader(std::string header,char delim){

    std::map<std::string, std::string> result;

    auto commaSeparated = stringutil::split(header,delim);

    for (auto headerItem : commaSeparated){

        if (string(headerItem).indexOf("=")!=0){
            auto trimmed = string(headerItem).trim();
            std::string key   = stringutil::replaceAll(trimmed.toStdString().substr(0,trimmed.indexOf("=")),"\"","");
            std::string value = stringutil::replaceAll(trimmed.toStdString().substr(trimmed.indexOf("=")+1),"\"","");
            result[key]=value;
        }
    }
    return result;
}

/**
 * @brief DigestManager::set_show_algorithm
 *      algorithm used in digest authentication will be sent to client if set to true (default)
 * @param show_in_response
 */
void DigestManager::set_show_algorithm(bool show_in_response){
    show_algorithm=show_in_response;
}

/**
 * @brief DigestManager::generateHandshakeProcess
 *      build digest authentication request
 * @param host
 *      host address
 * @param path_realm
 *      realm used for authentication
 * @return
 *      http response
 */
DigestInfo DigestManager::generateHandshakeProcess(std::string host,std::string path_realm){

    std::string serverIp ="";

    if (host.find(':') != std::string::npos){
        serverIp=string(host.substr(0,string(host.data()).indexOf(":")).data()).trim().toStdString();
    }
    else{
        serverIp=string(host.data()).trim().toStdString();
    }

    std::string realm = string(string(path_realm.data()).toUtf8().toBase64()).toStdString();

    std::string randomStr = generateRandomNum(62);

    auto nonce_timestamp = getTimeSinceEpoch<std::chrono::milliseconds>();

    //generate nonce (algorithm used for generating nonce is not part of RFC)
    std::string nonce_value = string(Sha1(randomStr)).toStdString();

    std::string opaque = generateRandomNum(62);

    //to be sure opaque is unique
    while (nonce_map.find(opaque)!=nonce_map.end()){
        opaque = generateRandomNum(62);
    }

    auto current_date = getTimeSinceEpoch<std::chrono::milliseconds>();

    nonce nonce_item = {
        nonce_value,
        nonce_timestamp+nonce_timeout_millis,
        1,
        false,
        ""
    };

    //store nonce in map for keeping authentication information for this peer
    nonce_map[opaque]=nonce_item;

    std::string clientId = "";

    if (session_t==SESSION_COOKIE){

        clientId = generateRandomNum(62);

        //to be sure clientId is unique
        while (session_map.find(clientId)!=session_map.end()){
            clientId = generateRandomNum(62);
        }

        session_map[clientId].session_start_time=current_date;
        session_map[clientId].validity=false;

        nonce_map[opaque].session_id=clientId;
    }

    //std::string content = unauthorized_page;

    //stringstream contentLength;
    //contentLength << content.size();

    bool stale = nonce_map[opaque].stale;
    const char * stale_txt = "false";
    if (stale)
        stale_txt="true";

    //build digest www-authorization header
    std::string digest_header   = string("").toStdString()      + "Digest "
        + "realm=\""            + realm                          + "\","
        + "qop=\""              + "auth-int"                     + "\","
        + "nonce=\""            + nonce_map[opaque].nonce  + "\","
        + "opaque=\""           + opaque + "\","
        + "stale=\""            + stale_txt                      + "\"";

    //add algorithm if wanted (always set to true according rfc)
    if (show_algorithm) {

        std::string algo_str = "";

        if (algorithm==ALGO_MD5)
            algo_str="MD5";
        else if (algorithm==ALGO_SHA1)
            algo_str="SHA1";
        else if (algorithm == ALGO_SHA256)
            algo_str="SHA-256";

        digest_header+= string("").toStdString() + ",algorithm=\"" + algo_str  + "\"";
    }

    std::multimap<std::string, std::string> headers_ret{};

    headers_ret.insert(std::pair("WWW-Authenticate", digest_header));

    //build set of headers for digest response
    //std::string digestHandshake = string("HTTP/1.1 401 Unauthorized\r\n").toStdString()
    //    + "WWW-Authenticate: " + digest_header            + "\r\n"
    //    + "Content-Type: "     + "text/html"              + "\r\n";
    //"Content-Length: "   + contentLength.str()      + "\r\n";

    if (session_t==SESSION_COOKIE){

        //headers_ret["Set-Cookie"]="authentication=running";

        std::string cookie=string("").toStdString() + "HSID=" + clientId + "; Domain=" + serverIp + "; Path=/;";

        headers_ret.insert(std::pair("Set-Cookie", cookie));

        //digestHandshake+=string("").toStdString()
        //+ "Set-Cookie: "       + "authentication=running" + "\r\n"
        //+ "Set-Cookie: "       + "HSID=" + clientId + "; Domain=" + serverIp + "; Path=/; HttpOnly\r\n" + "\r\n";
    }
    /*
    else{
        digestHandshake+=string("\r\n").toStdString();
    }
    */

    DigestInfo response_digest(401,headers_ret);

    //std::string fullResponse = digestHandshake + content;

    return response_digest;
}

/**
 * @brief DigestManager::generateRandomNum
 *      Generate random alpha numeric string
 * @param num
 *      length of random string
 * @return
 */
std::string DigestManager::generateRandomNum(unsigned int num){

    std::string str;

    const char alphanum[] =
       "0123456789"
       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
       "abcdefghijklmnopqrstuvwxyz";

    int stringLength = sizeof(alphanum) - 1;

    for(unsigned int i = 0; i < num;i ++){

        str += alphanum[rand() % stringLength];
    }
    return str;
}

/**
 * @brief DigestManager::set_digest_listener
 *      Set digest listener
 * @param digest_listener
 */
void DigestManager::set_digest_listener(IDigestListener* digest_listener){
    this->digest_listener=digest_listener;
}

/**
 * @brief DigestManager::processDigestResponse
 *      Process digest response request to authenticate and validate session cookie
 *
 * @param authorizationHeader
 *      header Authorization
 * @param cookieHeader
 *      Cookie header
 * @param realm
 *      realm used for this authentication
 * @param http_method_requested
 *      http method requested by client
 * @param uri_requested
 *      uri requested by client
 * @return
 *      status code
 */
DigestInfo DigestManager::processDigestResponse(std::string authorizationHeader,std::string cookieHeader,std::string realm,std::string http_method_requested,std::string uri_requested){

    std::multimap<std::string, std::string> headers_ret{};

    std::map<std::string, std::string> subheaderList = splitHeader(stringutil::replaceAll(authorizationHeader, "Digest", ""),',');

    if (subheaderList.find("opaque")==subheaderList.end()){
        std::cout << "opaque property was not found in Authorization header response\n";
        DigestInfo digest_response(401,headers_ret);
        return digest_response;
    }

    std::string opaque = subheaderList["opaque"];

    //check if peer address has an entry in map
    if (nonce_map.find(opaque)==nonce_map.end()){
        std::cout << "nonce wasnt requested for this client\n";
        DigestInfo digest_response(401,headers_ret);
        return digest_response;
    }

    unsigned long nonce_timestamp = getTimeSinceEpoch<std::chrono::milliseconds>();

    //check nonce life time
    if (nonce_map[opaque].expiring_date<=nonce_timestamp){
        std::cout << "nonce expired\n";
        nonce_map.erase(opaque);
        DigestInfo digest_response(401,headers_ret);
        return digest_response;
    }

    if (digest_listener==0) {
        std::cout << "digest listener was not defined. Cant process digest response\n";
        DigestInfo digest_response(401,headers_ret);
        return digest_response;
    }

    if (authorizationHeader != "") {
//        Exemple => Digest realm="Ym91eWd1ZXN0ZWxlY29tYmhvbWUyMDE0AA==",
//                   qop="auth-int",
//                   nonce="f5cf2080b1352ef1c2a1aaa05bca0429",
//                   opaque="60ea993b2f20e83335d8d85c1cf054dc",
//                   username="qsd",
//                   uri="/login",
//                   response="4998d82770b0729dddf403f7d3b477bb",//                   nc=00000001,
//                   cnonce="testCnonce"

        //check authorization header properties
        if (subheaderList.find("cnonce")==subheaderList.end()){
            std::cout << "cnonce property was not found in Authorization header response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("username")==subheaderList.end()){
            std::cout << "username property was not found in Authorization header response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("response")==subheaderList.end()){
            std::cout << "response property was not found in Authorization header response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("qop")==subheaderList.end()){
            std::cout << "qop property was not found in Authorization header response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("nc")==subheaderList.end()){
            std::cout << "nc property was not found in Authorization header response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }
        else if (subheaderList.find("uri")==subheaderList.end()){
            std::cout << "uri property was not found in Authorization header response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        //check that qop value is correct
        if (subheaderList["qop"] != "auth-int"){
            std::cout << "qop value is incorrect in digest response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        std::stringstream nonce_count;
        nonce_count << nonce_map[opaque].nonce_count;
        std::string nonce_count_str = std::string(8 - nonce_count.str().length(), '0') + nonce_count.str();

        nonce_map[opaque].nonce_count++;

        //check that nonce count value is correct
        if (subheaderList["nc"] != nonce_count_str){
            std::cout << "nc value is incorrect in Authorization header response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        //check that request uri is right
        if (subheaderList["uri"] != uri_requested){
            std::cout << "uri value is incorrect in Authorization header response\n";
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        std::string cnonce   = subheaderList["cnonce"];
        std::string username = subheaderList["username"];

        std::string hash2Str = string(http_method_requested.data()).toStdString()  + ":" + uri_requested;
        std::string hash1 = digest_listener->get_hash_for_username(username,realm,algorithm);

        //check authentication data is not empty
        if (hash1 == ""){
            std::cout << "empty hash returned from digest listener for username : " << username.data()  << '\n';
            DigestInfo digest_response(401,headers_ret);
            return digest_response;
        }

        if (session_t==SESSION_DIGEST){
            hash1+=string("").toStdString() + ":" + nonce_map[opaque].nonce + ":" + cnonce;
        }

        std::string hash2="";
        if (algorithm==ALGO_MD5)
            hash2 = Md5(hash2Str);
        else if (algorithm==ALGO_SHA1)
            hash2 = Sha1(hash2Str);
        else if (algorithm==ALGO_SHA256)
            hash2 = Sha256(hash2Str);

        std::string hash3Str = string(hash1.data()).toLower().toStdString() + ":" +
                nonce_map[opaque].nonce  + ":" + nonce_count_str + ":" + cnonce + ":" + "auth-int" + ":" + string(hash2.data()).toLower().toStdString();

        std::string hash3 ="";
        if (algorithm==ALGO_MD5)
            hash3 = Md5(hash3Str);
        else if (algorithm==ALGO_SHA1)
            hash3 = Sha1(hash3Str);
        else if (algorithm==ALGO_SHA256)
            hash3 = Sha256(hash3Str);

        std::string clientResponse = subheaderList["response"];

        if (session_t==SESSION_COOKIE) {

            if (cookieHeader != "") {

                std::map<std::string, std::string> cookiesList   = splitHeader(cookieHeader       ,';');

                //check cookie header properties
                if (session_t==SESSION_COOKIE && cookiesList.find("HSID")==subheaderList.end()){
                    std::cout << "HSID property was not found in Cookie header response\n";
                    DigestInfo digest_response(401,headers_ret);
                    return digest_response;
                }

                std::string hsid = cookiesList["HSID"];

                if (session_map.find(hsid)!=session_map.end()){

                    if (nonce_map[opaque].session_id == hsid){

                        std::cout << "Digest client recognized. Checking for response...\n";

                        if (string(hash3.data()).toUpper().toLocal8Bit() == string(clientResponse.data()).toUpper().toLocal8Bit()){

                            std::cout << "Digest client with HSID " << hsid << " has been connected successfully\n";
                            session_map[hsid].validity=true;
                            nonce_map.erase(opaque);
                            DigestInfo digest_response(200,headers_ret);
                            return digest_response;
                        }
                        else{
                            std::cout << "Digest client with HSID " << hsid << " was rejected :/\n";
                        }
                    }
                    else{
                        std::cout << "Digest response was received for unregistered digest client !\n";
                    }
                }
                else{
                    std::cout << "unknown session id\n";
                }
            }
            else{
                std::cout << "No cookie header found\n";
            }
        }
        else {

            std::cout << "[DIGEST SESSION] Digest client recognized. Checking for response...\n";

            if (string(hash3.data()).toUpper().toLocal8Bit() == string(clientResponse.data()).toUpper().toLocal8Bit()){

                DigestInfo digest_response(200,headers_ret);
                return digest_response;
            }
            else{
                std::cout << "Digest session authentication failed hash is invalid\n";
            }
        }
    }
    else {
        std::cout<< "Error while reading digest response...";
    }

    DigestInfo digest_response(401,headers_ret);
    return digest_response;
}
