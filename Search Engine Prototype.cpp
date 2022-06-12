#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
using namespace std;

char unique_terms[10000][150], stop_words[500][20], doc_list[50][30];      //declarations
char temp[150], TEMP[150], user_term[150] = "", termslocated[2000][150];
int term_frequency_in_all_documents[11][100];
int y = 0, x = 0, TermId, docID = 0;
bool stop = false, found = false;

int my_strlen(char arr[])
{
	int i = 0;
	for (i = 0; arr[i] != '\0'; i++);
	return i;
}

int my_strcmp(char arr1[], char arr2[])
{
	int len1 = my_strlen(arr1), len2 = my_strlen(arr2);
	for (int i = 0, j = 0; arr1[i] != '\0' && arr2[j] != '\0'; i++, j++)
	{
		if (arr1[i] > arr2[i])
		{
			return 1;
		}
		if (arr1[i] < arr2[i])
		{
			return -1;
		}
	}
	if (len1 < len2)
	{
		return -1;
	}
	else if (len1 > len2)
	{
		return 1;
	}
	else
		return 0;
}

void read_stop_words()
{
	for (int i = 0; i < 500; i++)            //initiallizing the array
	{
		for (int j = 0; j < 20; j++)
		{
			stop_words[i][j] = '\0';
		}
	}
	int i = 0;
	ifstream fin("stoplist.txt");            //reading from file
	while (fin.eof() == false)
	{
		fin >> stop_words[i];
		i++;
	}
	fin.close();

	bool stop = false;                      //outputing stopwords array
}

void read_docs_list()
{
	for (int i = 0; i < 50; i++)            //initiallizing the array
	{
		for (int j = 0; j < 30; j++)
		{
			doc_list[i][j] = '\0';
		}
	}

	int i = 0;
	ifstream fin("docslist.txt");            //reading from file
	while (fin.eof() == false)
	{
		fin >> doc_list[i];
		i++;
	}
	fin.close();
}

void write_docs_with_IDs()
{
	ofstream fout("docids.txt");
	if (fout.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			if (doc_list[i][0] != '\0')
			{
				fout << i << "\t" << doc_list[i] << "\n";
			}
		}
	}
	else
	{
		cout << "File not opened";
	}
	fout.close();
}

int check_in_stopwords(char word[])
{
	for (int i = 0; stop_words[i][0] != '\0'; i++)            //checking words in stopwords
	{
		for (int j = 0; stop_words[i][j] != '\0'; j++)
		{
			if (my_strcmp(stop_words[i], word) == 0)   //found
			{
				return 1;
			}
		}
	}
	return 0;  //not found
}

int check_in_unique_terms(char word[])
{
	for (int i = 0; unique_terms[i][0] != '\0'; i++)            //checking words in stopwords
	{
		for (int j = 0; unique_terms[i][j] != '\0'; j++)
		{
			if (my_strcmp(unique_terms[i], word) == 0)    //found
			{
				return 1;
			}
		}
	}
	return 0; //not found
}

void convert_to_tokens(char docname[30])
{
	ifstream fin(docname);
	while (fin.eof() == false)                 //reading each file's data in temp array
	{
		for (int i = 0; i < 150; i++)             //initiallizing temp array
		{
			temp[i] = '\0';
		}
		for (int i = 0; i < 150; i++)
		{
			TEMP[i] = '\0';
		}
		fin >> temp;
		for (int i = 0, y = 0; temp[i] != '\0'; i++)                    //removing special char and numbers
		{
			if (temp[i] >= 'A' && temp[i] <= 'Z' || temp[i] >= 'a' && temp[i] <= 'z')
			{
				TEMP[y] = temp[i];
				y++;
			}
		}
		for (int i = 0; TEMP[i] != '\0'; i++)                  //making lowercase
		{
			if (TEMP[i] >= 'A' && TEMP[i] <= 'Z')
			{
				TEMP[i] = char(int(TEMP[i]) + 32);
			}
		}
		int j;                                                //each term in TEMP is checked in stopwords array and
		for (j = 0; unique_terms[j][0] != '\0'; j++);    	  //uniqueterms array...if not found then put in unique terms
		if (check_in_stopwords(TEMP) == 0 && check_in_unique_terms(TEMP) == 0)
		{
			strcpy(unique_terms[j], TEMP);
		}
	}
}

void sort_terms_alphabetically(char arr[][150])
{
	char temp[150];
	int size = 0;
	for (size; arr[size][0] != '\0' && size < 10000; size++);   //finding occupied length of array
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			if (my_strcmp(arr[j], arr[j + 1]) > 0)      // if term of left is greater than right side term
			{
				strcpy(temp, arr[j]);                //swapping
				strcpy(arr[j], arr[j + 1]);
				strcpy(arr[j + 1], temp);
			}
		}
	}
}

void write_term_IDs()
{
	ofstream fout("termids.txt");
	if (fout.is_open())
	{
		for (int i = 0; unique_terms[i][0] != '\0'; i++)
		{
			fout << i << "\t" << unique_terms[i] << "\n";
		}
	}
	else
	{
		cout << "File not opened";
	}

	fout.close();
}

void write_doc_index()
{
	char term[200], TERM[150];
	int positions[100];             //to store all positions at which the term occurs
	int j = 0, i = 0; //pointer for term position in file
	ofstream fout("doc_index2.txt");

	if (fout.is_open() == true)
	{
		for (j = 0; doc_list[j][0] != '\0'; j++)         //one document taken at a time
		{
			for (int i = 0; unique_terms[i][0] != '\0'; i++)        //one unique term taken at a time and compared with all terms of the file
			{
				ifstream fin(doc_list[j]);
				int pointer = -1, k = 0;
				bool flag = false;
				for (int x = 0; x < 100; x++)                 //initiallizing positions array
				{
					positions[x] = 0;
				}
				while (fin.eof() == false)
				{
					for (int i = 0; i < 200; i++)             //initiallizing term arrays
					{
						term[i] = '\0';
					}
					for (int i = 0; i < 150; i++)
					{
						TERM[i] = '\0';
					}

					fin >> term;      //extracting each word
					pointer++;        //the word position counter
					for (int i = 0, y = 0; term[i] != '\0'; i++)          //removing special char and numbers
					{
						if (term[i] >= 'A' && term[i] <= 'Z' || term[i] >= 'a' && term[i] <= 'z')
						{
							TERM[y] = term[i];
							y++;
						}
					}
					for (int i = 0; TERM[i] != '\0'; i++)        //making lowercase
					{

						if (TERM[i] >= 'A' && TERM[i] <= 'Z')
						{
							TERM[i] = char(int(TERM[i]) + 32);
						}
					}

					if (my_strcmp(TERM, unique_terms[i]) == 0) //is equal?
					{
						positions[k] = pointer;
						k++;
						flag = true;  //atleast one position found
					}
				}
				//after the file search is complete
				if (flag == true)
				{
					int x = 0;
					fout << j << "\t" << i << "\t";  //docid and termid
					for (x = 0; x < k - 1; x++)
					{
						fout << positions[x] << "\t"; //positions where term was found
					}
					fout << positions[x] << "\n";
				}
				fin.close();
			}
		}
		fout << "\0";
	}
	fout.close();
}

void print1DArray(int array[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (array[i] != -1)
		{
			cout << array[i] << "\t";
		}
	}
}

void print2DArray(int array[10][100], int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		if (array[i][0] != -1)
		{
			cout << "Document Number " << i + 1 << endl;
			print1DArray(array[i], cols);
			cout << endl;
		}

	}
}

int get_term_id(char term[])
{
	int len = my_strlen(term);
	bool flag = false, found = false;

	for (int i = 0; i < len; i++)
	{
		if (term[i] >= 'A' && term[i] <= 'Z')
		{
			term[i] = (int(term[i]) + 32);
		}
	}
	for (int i = 0; stop_words[i][0] != '\0'; i++)
	{
		if (my_strcmp(term, stop_words[i]) == 0)
		{
			flag = true;
			break;
		}
	}
	if (found == false)
	{
		for (int i = 0; unique_terms[i][0] != '\0'; i++)
		{
			if (my_strcmp(term, unique_terms[i]) == 0)
			{
				return i; //termID
			}
		}
	}
	if (found == false && flag == true)  //found in stopwords and not found in unique terms
	{
		return -1;
	}
	return -1;


}

void find_term_in_document(int term_ID, int documentID)
{
	int i = 0;
	ifstream file_doc_index("doc_index2.txt");
	if (file_doc_index.is_open()) {
		int docID = 0, termID = -1;
		while (!file_doc_index.eof())
		{
			file_doc_index >> docID >> termID;
			if (docID > documentID)
				break;
			if (docID == documentID && termID == term_ID) {

				int position = -1;
				int tab = int('\t');

				char ch = file_doc_index.peek();
				while (ch != '\n')
				{
					file_doc_index >> position;
					term_frequency_in_all_documents[documentID][i] = position;
					i++;
					ch = file_doc_index.peek();
				}
			}
			else {
				int ignore = 0;

				int ch = file_doc_index.peek();
				while (10 != ch && !file_doc_index.eof())
				{
					file_doc_index >> ignore;
					ch = file_doc_index.peek();
				}
			}
		}
	}
	else
	{
		file_doc_index.close();
		cout << "The termID doesnot exit in documentID 0 " << endl;
	}
}

int get_sub_term_id(char term[])
{
	int choice = 0;
	bool none_found = true;
	int validID[2000];
	for (int j = 0; j < 2000; j++)
	{
		validID[j] = -1;
	}
	for (int i = 0; i < 2000; i++)
	{
		for (int j = 0; j < 150; j++)
		{
			termslocated[i][j] = '\0';
		}
	}
	for (int i = 0; unique_terms[i][0] != '\0'; i++)
	{
		if (string(unique_terms[i]).find(term) == 0)  //prefix found in another term
		{
			strcpy(termslocated[i], unique_terms[i]);
		}
	}
	int k = 0;
	for (int i = 0; i < 2000; i++)
	{
		if (termslocated[i][0] != '\0') {
			cout << i << "\t";
			validID[k] = i; //for validation purpose
			k++;
			for (int j = 0; termslocated[i][j] != '\0'; j++)
			{
				cout << termslocated[i][j];
			}
			cout << endl;
			none_found = false;
		}
	}
	if (none_found == false)
	{
		cout << "Select the Id of term you wanted to search" << endl;
		while (validID[0] != -1) //atleast has one id
		{
			cout << "Choice of TermId? ";
			cin >> choice;
			for (int i = 0; validID[i] != -1; i++)  //vali
			{
				if (choice == validID[i])
				{
					return choice;
				}
			}
			cout << "----OOps you entered the incorrect TermId----" << endl;
		}
	}
	return -1; //goodnotes
}

int hamming_distance(char term1[], char term2[])
{
	int i = 0, dist = 0;
	while (term1[i] != '\0')
	{
		if (term1[i] != term2[i])
			dist++;
		i++;
	}
	return dist;
}

int get_Wrong_Word_ID(char wrong_word[])
{
	int choice = 0, length1 = my_strlen(wrong_word), length2 = -1;
	bool none_found = true;
	int validID[2000];
	for (int j = 0; j < 2000; j++)
	{
		validID[j] = -1;
	}
	for (int i = 0; i < 2000; i++)
	{
		for (int j = 0; j < 150; j++)
		{
			termslocated[i][j] = '\0';
		}
	}
	for (int i = 0; unique_terms[i][0] != '\0'; i++)
	{
		length2 = my_strlen(unique_terms[i]);
		if (length1 == length2 && hamming_distance(unique_terms[i], wrong_word) < 3)
		{
			strcpy(termslocated[i], unique_terms[i]);
		}
	}
	int k = 0;
	for (int i = 0; i < 2000; i++)
	{
		if (termslocated[i][0] != '\0') {
			cout << i << "\t";
			validID[k] = i;
			k++;
			for (int j = 0; termslocated[i][j] != '\0'; j++)
			{
				cout << termslocated[i][j];
			}
			cout << endl;
			none_found = false;
		}
	}
	if (none_found == false)
	{
		cout << "Enter the Id of term you wanted to search" << endl;
		while (validID[0] != -1) //atleast has one id
		{
			cout << "Choice of TermId? ";
			cin >> choice;
			for (int i = 0; validID[i] != -1; i++)  //validation check
			{
				if (choice == validID[i])
				{
					return choice;
				}
			}
			cout << "----OOps you entered the incorrect TermId----" << endl;
		}
	}
	else
		return -1;
}

void read_unique_terms()
{
	ifstream fin("termids_temp.txt");
	int i = 0, id = 0;
	while (fin.eof() == false)
	{
		fin >> id >> unique_terms[i];
		i++;
	}
}

int main()
{
	int choice;
	char str[200];
	bool stop_please = false;
	time_t my_time = time(NULL);
	cout << "Hello & Welcome!..." << endl;
	while (stop_please == false)
	{
		while (true)
		{
			cout << "Enter 0 to Skip Part-1 OR Press Any Key from 1 to 9 to Run Without Skipping " << endl;
			cin >> choice;
			if (choice >= 0 && choice <= 9)  //number range validation
			{
				break;
			}
		}
		if (choice == 0)
		{
			cout << "Currently calling read_unique_terms() function" << endl;
			read_unique_terms();
		}
		else
		{
			cout << "Currently calling read_stop_words() function" << endl;
			read_stop_words();
			cout << "Currently calling read_docs_list() function" << endl;
			read_docs_list();
			cout << "Currently calling write_docs_with_IDs() function" << endl;
			write_docs_with_IDs();
			cout << "Currently calling convert_to_tokens() function" << endl;
			for (int i = 0; i <= 10; i++)
			{
				convert_to_tokens(doc_list[i]);
			}
			cout << "Currently calling sort_terms_alphabetically() function" << endl;
			sort_terms_alphabetically(unique_terms);
			cout << "Currently calling write_term_IDs() function" << endl;
			write_term_IDs();
			cout << "Currently calling write_doc_index() function" << endl;
			write_doc_index();
		}

		cout << "Enter the Term you want to Search" << endl;
		cin >> user_term;
		cout << "\n";
		cout << "Currently calling get_term_id() function" << endl;
		TermId = get_term_id(user_term);
		if (TermId == -1)
		{
			cout << ".\nCurrently calling get_sub_term_id() function" << endl;
			TermId = get_sub_term_id(user_term);
			if (TermId == -1)
			{
				cout << ".\nCurrently calling get_Wrong_Word_ID() function" << endl;
				TermId = get_Wrong_Word_ID(user_term);
				if (TermId == -1)
				{
					cout << ",\n,\noops, Could not find any Search Results..." << endl;
				}
			}
		}
		if (TermId >= 0)  //TermID is searched in documents only if it is valid
		{
			for (int i = 0; i < 11; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					term_frequency_in_all_documents[i][j] = -1;
				}
			}
			cout << "Currently calling find_term_in_document() function" << endl;
			for (int i = 0; i <= 10; i++)
			{
				docID = i;
				find_term_in_document(TermId, docID);
			}
			cout << "Currently calling print2DArray() function\n" << endl;
			print2DArray(term_frequency_in_all_documents, 11, 100);
		}
		ofstream history;
		history.open("history.txt", std::ios::app);
		if (history.is_open())     //appending in history.txt
		{
			history << user_term << "\t" << ctime(&my_time) << "\n";
		}
		history.close();

		cout << "\n Do You want to Search another Term ?" << endl;
		while (true)
		{
			cout << "Press any key from 1 to 9 to Continue OR Press 0 to Finish Searches & View History" << endl;
			cin >> choice;
			if (choice >= 0 && choice <= 9)  //number range validation
			{
				break;
			}
		}
		if (choice == 0)
		{
			cout << "This is you Search History \n" << endl;
			ifstream file("history.txt");    //printing search history
			if (file.is_open() == true)
			{
				while (file.eof() == false)
				{
					file.getline(str, 200);
					cout << str << endl;
				}
			}
			file.close();
			stop_please = true;
		}
	}
	cout << "BYE! See You Next Time..." << endl;
	return 0;
}