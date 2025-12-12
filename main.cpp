#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <dirent.h>

using namespace std;

string int2str(int &i) {
  string s;
  stringstream ss(s);
  ss << i;
  return ss.str();
}

int str2int(const string &str) {
  stringstream ss(str);
  int num;
  ss >> num;
  return num;
}

void brkts(string &line, bool &clr) {
  if(line.find("/*") != string::npos && line.find("*/") != string::npos && line.find("/*") < line.find("*/")) {
    string tmp_1;
    string tmp_2;
    tmp_1.assign(line, 0, line.find("/*"));
    tmp_2.assign(line, line.find("*/")+2, line.length()-line.find("*/")-2);
    line.clear();
    line.append(tmp_1);
    line.append(tmp_2);
  } else if(line.find("/*") != string::npos && line.find("*/") == string::npos) {
    string tmp_3;
    tmp_3.assign(line, 0, line.find("/*"));
    line.clear();
    line.append(tmp_3);
    clr = 1;
  } else if(line.find("*/") != string::npos && line.find("/*") == string::npos) {
    string tmp_4;
    tmp_4.assign(line, line.find("*/")+2, line.length()-line.find("*/")-2);
    line.clear();
    line.append(tmp_4);
    clr = 0;
  } else if(clr == 1) {
    line.clear();
  }
}

void kill_dummy(string &line, bool &clr) {
  if(line.find("//") != string::npos) {
    string tmp = line;
    line.clear();
    line.assign(tmp, 0, tmp.find("//"));
  }
  brkts(line, clr);
}

void par(vector<string> &res, string &word) {
  string result;
  stringstream input(word);
  while(input >> result)
    res.push_back(result);
}

void par_1(vector<string> &tmps, string tt, string key) {
  size_t pos = 0;
  size_t ini = -1;
  string out;
  while((pos = tt.find(key, pos)) != string::npos) {
    out.assign(tt, ini+1, pos-ini-1);
    if(!out.empty()) tmps.push_back(out);
    ini = pos;
    pos += key.length();
  }
  out.assign(tt, ini+1, pos-ini-1);
  if(!out.empty()) tmps.push_back(out);
  out.clear();
}

string abs(string curr, string str) {
  vector<string> curr_par = vector<string> ();
  par_1(curr_par, curr, "/");
  vector<string> str_par = vector<string> ();
  par_1(str_par, str, "/");

  vector<string> tmps = vector<string> ();
  if(str.find(".") == 0) {
    for(int i = 0; i < curr_par.size(); i++)
      tmps.push_back(curr_par[i]);
    for(int i = 0; i < str_par.size(); i++) {
      if(str_par[i] == ".")
        continue;
      else if(str_par[i] == "..")
        tmps.pop_back();
      else
        tmps.push_back(str_par[i]);
    }
  } else if(str.find("/") == 0) {
    for(int i = 0; i < str_par.size(); i++) {
      if(str_par[i] == ".")
        continue;
      else if(str_par[i] == "..")
        tmps.pop_back();
      else
        tmps.push_back(str_par[i]);
    }
  } else {
    for(int i = 0; i < curr_par.size(); i++)
      tmps.push_back(curr_par[i]);
    tmps.push_back(str);
  }

  string str_abs;
  for(int i = 0; i < tmps.size(); i++) {
    str_abs += '/';
    str_abs += tmps[i];
  }
  return str_abs;
}

void re(string &str, const string &key, const string &from, const string &to) {
  if(from.empty()) return;
  size_t start_pos = 0;
  if(str.find(key) == string::npos) return;
  else {
    while((start_pos = str.find(from, start_pos)) != string::npos) {
      str.replace(start_pos, from.length(), to);
      start_pos != to.length();
    }
  }
}

const class nullptr_t {
public:
  template<class T>
  inline operator T*() const {
    return 0;
  }
  template<class C, class T>
  inline operator T C::*() const {
    return 0;
  }
private:
  void operator&() const;
} nullptr = {};

string getEnv(const string &var) {
  const char *val = getenv(var.c_str());
  if(val == nullptr) return "";
  else return val;
}

string read_env_par(string str) {
  vector<string> re_s = vector<string> ();
  int s = 0;
  string tmp;
  for(string::iterator it = str.begin(); it != str.end(); it++) {
    if(s == 0) {
      if(*it == '$')
        s = 1;
    } else if(s == 1) {
      if(*it != '{') {
        string ss(1, *it);
        tmp.append(ss);
      }
      s = 2;
    } else if(s == 2) {
      if(*it == '/' || *it == '}') {
        re_s.push_back(tmp);
        tmp.clear();
        s = 0;
      } else if(it+1 == str.end()) {
        string ss(1, *it);
        tmp.append(ss);
        re_s.push_back(tmp);
        tmp.clear();
        s = 0;
      } else {
        string ss(1, *it);
        tmp.append(ss);
      }
    }
  }

  map<string, string> ans;
  for(int i = 0; i < re_s.size(); i++)
    if(getEnv(re_s[i]) != "")
      ans[re_s[i]] = getEnv(re_s[i]);
  string str1 = str;
  for(map<string, string>::iterator it = ans.begin(); it != ans.end(); it++) {
    string tmp1 = "$";
    tmp1.append(it->first);
    re(str1, "", tmp1, it->second);
    string tmp2 = "${";
    tmp2.append(it->first);
    tmp2.append("}");
    re(str1, "", tmp2, it->second);
  }
  ans.clear();
  re_s.clear();
  return str1;
}

bool get_sons(vector<string> &sons, string root, string father) {
  ifstream fr(father.c_str());
  if(!fr) {
    cout << "Error: " << father << endl;
    return 0;
  } else {
    string line;
    bool clr = 0;
    bool mf = 0;
    while(!fr.eof()) {
      getline(fr, line);
      kill_dummy(line, clr);
      if(line.empty()) continue;

      if(line.find("-f") != string::npos) {
        vector<string> tmps = vector<string> ();
        par(tmps, line);
        if(tmps[0] == "-f") {
          if(tmps.size() != 2) continue;
          mf = 1;
          string tmp = tmps[1];
          if(tmps[1].find("$") != string::npos) {
            tmp.clear();
            tmp = read_env_par(tmps[1]);
          }
          sons.push_back(abs(root, tmp));
        }
        tmps.clear();
      }
    }
    fr.close();
    return mf;
  }
}

void jia(string &str, const string &key, const string &f0, const string &f1) {
  if(size_t found = str.find(key) == string::npos) {
    str.erase(str.begin(), str.end());
    return;
  } else {
    int pos = str.find(f0)+f0.length();
    int l = str.find(f1, pos)-pos;
    string tmp_str = str;
    str.assign(tmp_str, pos, l);
  }
}

bool get_sons_1(vector<string> &sons, string father) {
  ifstream fr(father.c_str());
  if(!fr) {
    cout << "Error: " << father << endl;
    return 0;
  } else {
    string line;
    bool clr = 0;
    while(!fr.eof()) {
      getline(fr, line);
      kill_dummy(line, clr);
      if(line.empty()) continue;

      if(line.find("include") != string::npos) {
        vector<string> tmps = vector<string> ();
        par(tmps, line);
        if(tmps[0] == "include") {
          string tmp = tmps[1];
          jia(tmp, "", "\"", "\"");
          sons.push_back(tmp);
        }
        tmps.clear();
      }
    }
    fr.close();
    return 1;
  }
}

bool is_file(string name) {
  return (access(name.c_str(), F_OK) != -1);
}

void filter(vector<string> &in) {
  set<string> seen;
  vector<string> out;

  for(int i = 0; i < in.size(); i++) {
    if(in[i].empty()) continue;

    const string &s = in[i];
    if(seen.find(s) == seen.end()) {
      seen.insert(s);
      out.push_back(s);
    }
  }

  in.clear();
  for(int i = 0; i < out.size(); i++)
    in.push_back(out[i]);
}

string pick_one(string word, int num) {
  string result;
  stringstream input(word);
  int i = 0;
  while(input >> result) {
    if(i == num)
      return result;
    i++;
  }
}

void now(string &all) {
  struct tm *T;
  time_t t;
  time(&t);
  T = localtime(&t);
  vector<int> pp = vector<int> ();
  pp.push_back(T->tm_year+1900);
  pp.push_back(T->tm_mon+1);
  pp.push_back(T->tm_mday);
  pp.push_back(T->tm_hour);
  pp.push_back(T->tm_min);
  pp.push_back(T->tm_sec);

  for(int i = 0; i < pp.size(); i++) {
    string tmp = int2str(pp[i]);
    string zero = "0";
    string dash = "_";
    if(tmp.length() == 1)
      tmp = zero+tmp;
    if(i == 3)
      tmp = dash+tmp;
    all.append(tmp);
    tmp.clear();
  }
  pp.clear();
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(0);

  if (argv[1] == NULL) {
    cout << "Error: no input files" << endl;
    return 0;
  }
  vector<string> argvs = vector<string>();
  int num_argv = 0;
  while (argv[num_argv] != NULL) {
    argvs.push_back(argv[num_argv]);
    num_argv++;
  }
  string filelist = argvs[1];
  // string filelist = "/usrhome/terryy/cpp/verilog.f";
  string root = filelist.substr(0, filelist.rfind("/"));

  // vector<int> loc = vector<int> ();
  vector<string> files = vector<string>();

  queue<string> qq;
  qq.push(filelist);
  // int lv = 0;
  // loc.push_back(lv);
  files.push_back(filelist);

  while (!qq.empty()) {
    int sz = qq.size();
    for (int i = 0; i < sz; i++) {
      string curr = qq.front(); qq.pop();
      vector<string> sons = vector<string>();
      if (!get_sons(sons, root, curr)) continue;
      for (int j = 0; j < sons.size(); j++) {
        qq.push(sons[j]);
        // loc.push_back(lv);
        files.push_back(sons[j]);
      }
    }
    // lv++;
  }

  vector<string> unknown = vector<string>();
  vector<string> incdir = vector<string>();
  vector<string> verilog = vector<string>();

  for (int i = 0; i < files.size(); i++) {
    ifstream fr0(files[i].c_str());
    if (!fr0) cout << "Error: " << files[i] << endl;
    else {
      string line;
      bool clr = 0;
      while (!fr0.eof()) {
        getline(fr0, line);
        kill_dummy(line, clr);
        if (line.empty()) continue;

        vector<string> tmps = vector<string>();
        par(tmps, line);

        if ((tmps.size() != 1) || (line.find("+.") != string::npos) || (line.find("+define+") != string::npos)) {
          unknown.push_back(line);
          continue;
        }
        string tmp = tmps[0];
        re(tmp, "", " ", "");
        re(tmp, "", "\t", "");
        if (tmp.find("$") != string::npos) {
          string tmp1 = tmp;
          tmp = read_env_par(tmp1);
        }
        if (tmp.find("+incdir+") == 0) {
          re(tmp, "+incdir+", "+incdir+", "");
          if (tmp.rfind("/") == tmp.size() - 1) {
            string tmp1 = tmp;
            tmp = tmp1.substr(0, tmp1.rfind("/"));
          }
          incdir.push_back(abs(root, tmp));
        } else {
          verilog.push_back(abs(root, tmp));
        }
      }
      fr0.close();
    }
  }

  incdir.push_back(root);
  filter(unknown);
  filter(incdir);
  filter(verilog);

  // for(int i = 0; i < incdir.size(); i++)
  // cout << incdir[i] << endl;

  vector<string> incdir_all = vector<string>();
  vector<string> warning = vector<string>();

  queue<string> qq_1;
  for (int i = 0; i < verilog.size(); i++) qq_1.push(verilog[i]);

  set<string> incdir_all_seen = set<string>();
  while (!qq_1.empty()) {
    string curr = qq_1.front(); qq_1.pop();
    vector<string> sons = vector<string>();
    if (!get_sons_1(sons, curr)) continue;

    for (int i = 0; i < sons.size(); i++) {
      bool ok_sons = 0;
      for (int j = 0; j < incdir.size(); j++) {
        string tmp;
        if (is_file(incdir[j] + "/" + sons[i])) {
          ok_sons = 1;
          tmp = incdir[j] + "/" + sons[i];
        } else if (is_file(sons[i])) {
          ok_sons = 1;
          tmp = sons[i];
        } else {
          continue;
        }

        const string &s = tmp;
        if (incdir_all_seen.find(s) == incdir_all_seen.end()) {
          incdir_all.push_back(tmp);
          qq_1.push(abs(root, tmp));
          incdir_all_seen.insert(s);
        }
        break;
      }
      if (!ok_sons) {
        string tmp1 = "Warning: ";
        tmp1 += sons[i];
        tmp1 += " @";
        tmp1 += curr;
        warning.push_back(tmp1);
      }
    }
  }
  filter(warning);

  for (int i = 0; i < warning.size(); i++) cout << warning[i] << endl;

  for (int i = 0; i < verilog.size(); i++) {
    for (int j = 0; j < incdir_all.size(); j++) {
      if (incdir_all[j] == verilog[i]) {
        verilog[i].clear();
        break;
      }
    }
  }
  filter(verilog);

  vector<string> pack_name = vector<string>();
  vector<string> pack_file = vector<string>();

  for (int i = 0; i < verilog.size(); i++) {
    ifstream fr1(verilog[i].c_str());
    if (!fr1) cout << "Error: " << verilog[i] << endl;
    else {
      string line;
      bool clr = 0;
      int cnt = 0;
      string tmp;
      while (!fr1.eof()) {
        getline(fr1, line);
        kill_dummy(line, clr);
        if (line.empty()) continue;

        if (cnt == 0) {
          if (line.find("package") != string::npos) {
            if (pick_one(line, 0) == "package") {
              tmp = line;
              re(tmp, "", "\t", "");
              re(tmp, "", " ", "");
              jia(tmp, "", "package", ";");
              cnt++;
            }
          }
        } else if (cnt == 1) {
          if (line.find("endpackage") != string::npos) {
            if ((pick_one(line, 0) == "endpackage") || (pick_one(line, 0) == "endpackage:")) {
              pack_name.push_back(tmp);
              pack_file.push_back(verilog[i]);
              break;
            }
          }
        }
      }
      fr1.close();
    }
  }

  vector<string> uu = vector<string>();
  vector<string> vv = vector<string>();
  vector<string> ww = vector<string>();

  for (int i = 0; i < verilog.size(); i++) {
    ifstream fr2(verilog[i].c_str());
    if (!fr2) cout << "Error: " << verilog[i] << endl;
    else {
      string line;
      bool clr = 0;
      set<string> cache = set<string>();

      while (!fr2.eof()) {
        getline(fr2, line);
        kill_dummy(line, clr);
        if (line.empty()) continue;

        if (line.find("::") != string::npos) {
          vector<string> tmps = vector<string>();
          par(tmps, line);
          for (int j = 0; j < tmps.size(); j++) {
            if (tmps[j].find("::") != string::npos) {
              string tt;
              if (line.find("import") != string::npos)
                tt.assign(tmps[j], 0, tmps[j].find("::"));
              else {
                string str = tmps[j];
                int str_end = tmps[j].find("::");
                int str_start = 0;
                for (int k = str_end - 1; k >= 0; k--) {
                  if (!isalpha(str[k]) && str[k] != '_' && !isdigit(str[k])) {
                    str_start = k + 1;
                    break;
                  }
                }
                tt.assign(str, str_start, str_end - str_start);
              }
              if (tt.empty()) continue;

              const string &s = tt;
              if (cache.find(s) == cache.end()) {
                bool ok = 0;
                for (int k = 0; k < pack_name.size(); k++) {
                  if (pack_file[k] == verilog[i]) {
                    uu.push_back(pack_name[k]);
                    ok = 1;
                    break;
                  }
                }
                if (!ok) uu.push_back("x");

                vv.push_back(tt);
                ww.push_back(verilog[i]);
                cache.insert(tt);
              }
            }
          }
        }
      }
      fr2.close();
    }
  }

  vector<string> uu_1 = vector<string>();
  vector<string> vv_1 = vector<string>();
  vector<string> ww_1 = vector<string>();
  set<string> warning_import_seen = set<string>();
  for (int i = 0; i < vv.size(); i++) {
    bool ok = 0;
    for (int j = 0; j < pack_name.size(); j++) {
      if (vv[i] == pack_name[j]) {
        uu_1.push_back(uu[i]);
        vv_1.push_back(vv[i]);
        ww_1.push_back(ww[i]);
        ok = 1;
        break;
      }
    }
    if (!ok) {
      string tmp = "Warning: import ";
      tmp.append(vv[i]);
      tmp.append(" @");
      tmp.append(ww[i]);
      const string &s = tmp;
      if (warning_import_seen.find(s) == warning_import_seen.end()) {
        warning_import_seen.insert(s);
        cout << tmp << endl;
      }
    }
  }

  int a[uu_1.size()];
  int b[vv_1.size()];
  for (int i = 0; i < uu_1.size(); i++) {
    a[i] = -1;
    b[i] = -1;
  }
  pack_name.push_back("x");
  pack_file.push_back("x");

  for (int i = 0; i < uu_1.size(); i++) {
    for (int j = 0; j < pack_name.size(); j++) {
      if (pack_name[j] == uu_1[i]) {
        a[i] = j;
        break;
      }
    }
    for (int j = 0; j < pack_name.size(); j++) {
      if (pack_name[j] == vv_1[i]) {
        b[i] = j;
        break;
      }
    }
  }

  int first[pack_name.size()];
  for (int i = 0; i < pack_name.size(); i++) first[i] = -1;
  int nxt[uu_1.size()];
  for (int i = 0; i < uu_1.size(); i++) {
    nxt[i] = first[a[i]];
    first[a[i]] = i;
  }

  vector<int> in_degree(pack_name.size(), 0);
  for (int i = 0; i < vv_1.size(); i++) in_degree[b[i]]++;

  queue<int> topo_sort;
  int visited = 0;
  for (int i = 0; i < in_degree.size(); i++) {
    if (in_degree[i] == 0) {
      topo_sort.push(i);
      visited++;
    }
  }

  vector<string> pp_name = vector<string>();
  vector<string> pp_file = vector<string>();
  while (!topo_sort.empty()) {
    int curr = topo_sort.front(); topo_sort.pop();
    if (pack_name[curr] != "x") {
      pp_name.push_back(pack_name[curr]);
      pp_file.push_back(pack_file[curr]);
    }
    int k = first[curr];
    while (k != -1) {
      in_degree[b[k]]--;
      if (in_degree[b[k]] == 0) {
        topo_sort.push(b[k]);
        visited++;
      }
      k = nxt[k];
    }
  }

  string tm_str;
  now(tm_str);
  tm_str += ".lst";

  ofstream fw(tm_str.c_str());
  for (int i = 0; i < unknown.size(); i++) fw << unknown[i] << endl;
  fw << endl;

  set<string> incdir_seen = set<string>();
  for (int i = 0; i < incdir_all.size(); i++) {
    string tmp = "+incdir+";
    tmp += incdir_all[i].substr(0, incdir_all[i].rfind("/"));
    const string &s = tmp;
    if (incdir_seen.find(s) == incdir_seen.end()) {
      incdir_seen.insert(s);
      fw << tmp << endl;
    }
  }
  fw << endl;

  set<string> verilog_seen = set<string>();
  for (int i = pp_file.size() - 1; i >= 0; i--) {
    const string &s = pp_file[i];
    verilog_seen.insert(s);
    fw << pp_file[i] << endl;
  }
  for (int i = 0; i < verilog.size(); i++) {
    const string &s = verilog[i];
    if (verilog_seen.find(s) == verilog_seen.end()) fw << verilog[i] << endl;
  }
  fw.close();

  return 0;
}
