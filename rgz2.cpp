#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <filesystem>
using namespace std;

class FileSearch
  {
    const char *separators{ " ,;:.\"!?'*\n" };
    vector <string> textsInFiles;
    vector <float> relevance;
    struct Scroll
      {
        string fileName;
      };
    Scroll *scroll;
    unsigned int numb;
    void way2file(string &name, string &way, int iter);
    public:
      FileSearch();
      FileSearch(unsigned int n)
        {
           numb = n;
           scroll = new Scroll[numb];
        }
      ~FileSearch()
        {
           delete[] scroll;
        }
      void fillScroll();
      void showScroll();
      void fileInOut();
      void wordSplit(vector<string>& words, string &text);
      unsigned countFindWords(string &text, string &findInText);
      void procPhrases(vector<string>& words, vector<string>& phrases, unsigned n);
      void procFrequencies(vector <float> &frequencies, string &text, string &findInText);
      void procRelevance();
      void filesRemove();
  };

void FileSearch::way2file(string &name, string &way, int iter)
  {
  	 string way_copy = way;
  	 name += ".txt";
     way_copy += name;
	 scroll[iter].fileName = way_copy;
  }

FileSearch::FileSearch()
  {
     cout<<"Input files number: ";
     cin>>numb;
     scroll = new Scroll[numb];
  }

void FileSearch::fillScroll()
  {
     fstream file;
     cout<<"Input files name"<<endl;
     for(unsigned int i=0;i<numb;i++)
       {
       	  string way, name;
       	  cout<<"File #"<<i+1<<endl;
       	  cout<<"Input way (without a filename): ";
       	  cin>>way;
          cout<<"Input name (without a suffix): ";
          cin>>name;
          way2file(name, way, i);
          cout<<scroll[i].fileName<<endl;
          file.open(scroll[i].fileName, fstream::in | fstream::out | fstream::app);
          if(!file) 
            {
              cout<<"ERROR! Try entering a different name";
              --i;
            }
           file.close();
       }
  }

void FileSearch::showScroll()
  {
     cout<<"Files in scroll"<<endl;
     for(unsigned int i=0;i<numb;i++)
       {
          cout<<"file #"<<i+1<<": ";
          cout<<scroll[i].fileName<<endl;
       }
  }

void FileSearch::fileInOut()
  {
     for(unsigned int i=0;i<numb;i++)
       {
          int choice;
          cout<<"1 - next action, 0 - exit, file #"<<i+1<<": ";
          cin>>choice;
          if(choice==1)
            {
              for(int j=0;j!=-1;j++)
                {
                  int choice2;
                  cout<<"1 - input, -1 - output, 0 - further: ";
                  cin>>choice2;
                  if(choice2==1)
                    {
                      ifstream fin;
                      fin.open(scroll[i].fileName,fstream::in | fstream::app);
                      string text, textSave;
                      while (getline(fin, text))
                        { 
                           cout<<text<<endl;
                           textSave += text + '\n';
						}
					  textsInFiles.push_back(textSave);
                      fin.close();
                    }
                     
                  else if(choice2==-1)
                    {
                      ofstream fout;
                      string text;
                      cout<<"Input text"<<endl;
                      getline(cin, text, '@');
                      fout.open(scroll[i].fileName, fstream::out | fstream::app);
                      if(fout.is_open())
                        {
                          fout<<text<<endl;
                        }
                      fout.close();     
                    }
                  else if(choice2==0)
                    {
                      j = -2;
                    }
                  else
                    {
                      --j;
                    }
                 }
              }
           
            else if(choice==0)
              {
                 i = numb;
              }
             else
               {
                  --i;
               }
         }
  }

void FileSearch::wordSplit(vector<string>& words, string &text)
  {
    stringstream ss(text);
    string buf;
    char *pch = strtok (const_cast<char*> (buf.data()), separators); 
 
    while (getline(ss, buf))
      {
         char* token = strtok(const_cast<char*> (buf.data()), separators);        
         while (token != nullptr)
           {         
             words.push_back(token);            
             token = strtok(nullptr, separators);               
           }
      }
      
  }

unsigned FileSearch::countFindWords(string &text, string &findInText)
  {
  	 unsigned count{};       
    for (unsigned i = 0; i <= text.length() - findInText.length();)
    {
      
        size_t position = text.find(findInText, i);
        
        if (position == std::string::npos) break;
        
        for(int j = 0;j < strlen(separators);j++)
          {
			 size_t bias = position + findInText.length();
          	 if((text[bias] == separators[j]) || (text[bias] == '\n') || (text[bias] == '\0'))
               ++count, j = strlen(separators);
		  }
        i = position + 1;
    }
     return count;
  }

void FileSearch::procPhrases(vector<string>& words, vector<string>& phrases, unsigned n)
  {
    for(unsigned int i = 0;i<(words.size()-n+1);i++)
      {
      	 phrases.push_back(words[i]);
      	
      	 for(unsigned int j = i+1;j<(i+n);j++)
      	   {
      	   	  
      	   	  phrases[i].append(" ");
      	   	  phrases[i] += words[j];
		   }
	  }
  }

void FileSearch::procFrequencies(vector <float> &frequencies, string &text, string &findInText)
  {
  	transform(text.begin(), text.end(), text.begin(), ::tolower);
    transform(findInText.begin(), findInText.end(), findInText.begin(), ::tolower);
  	vector<string> wordsInSub;
  	wordSplit(wordsInSub, findInText);
  	for(unsigned n = 1;n<=wordsInSub.size();n++)
  	  {
  	  	 float frequenc = 0;
  	  	 
  	     vector<string> phrases;
  	     procPhrases(wordsInSub, phrases, n);
  	     
  	     vector<string> wordsInText;
  	     wordSplit(wordsInText, text);
  	     vector<string> phrasesAll;
  	     procPhrases(wordsInText, phrasesAll, n);
  	     
  	     for(unsigned i = 0;i<phrases.size();i++)
  	       {
  	       	  unsigned count = countFindWords(text, phrases[i]);
  	          frequenc += (float) count / (float) phrasesAll.size();
           }
  	     frequencies.push_back(frequenc*100);
	  }
  }

void FileSearch::procRelevance()
  {
  	for(int c{};c!=-1;c++)
  	  {
  	  	int choice;
  	  	cout<<"1 - ask a request, 0 - exit"<<": ";
  	  	cin>>choice;
  	  	if(choice==1)
  	  	  {
  	        cin.get();
  	        string sub; 
  	        cout<<"input subtext: ";
  	        getline(cin, sub);
  	        vector <string> wordsInSubText;
  	        wordSplit(wordsInSubText, sub);
  	        vector <float> coefs{};
  	        coefs.resize(wordsInSubText.size());
  	        for(unsigned i{};i<coefs.size();i++)
  	          {
  	            cout<<"Input evaluation coefficient for frequency of phrases in "<<i+1;
  	            cout<<": "<<endl;
  	            cin>>coefs[i];
	          }
  	        for(unsigned i{};i<numb;i++)
  	          {
  	            vector <float> frequencies;
  	            procFrequencies(frequencies, textsInFiles[i], sub);
  	            float rel{};
  	            cout<<"File #"<<i+1<<endl;
  	            cout<<"Way: "<<scroll[i].fileName<<endl;
  	            for(unsigned j{};j<frequencies.size();j++)
  	              {
  	      	        rel += (frequencies[j]*coefs[j]);
		          }
		        rel = round(rel*10)/10;
		        relevance.push_back(rel);
		        cout<<"Relevance: "<<relevance[i]<<endl;
	          }
	        relevance.clear();
          }
        else if(choice==0)
          {
            c = -2;
          }
        else
          {
            --c;
          }
      }
  }

int main()
  {
     FileSearch f;
     f.fillScroll();    
     f.showScroll();
     f.fileInOut();
     f.procRelevance();
     return 0;
  }
         