#include <opencv2/highgui/highgui.hpp>
#include <windows.h>
#include <iostream>
#include <vector>

#define WIN32_LEAN_AND_MEAN 

using namespace std;
using namespace cv;

const wchar_t* extensions[] = 
{ 
  L".jpg",
  L".jpeg",
  L".png",
  L".bmp"
};

void PrintAllSupportedExtensions()
{
  cout << "Extensions in whitelist:\n";
  for (const auto extension : extensions)
  {
    wcout << extension << "\n";
  }
  cout << '\n';
}

bool IsFileExtensionInWhitelist(const wchar_t* fileName)
{
  for (const auto extension : extensions)
  {
    if (wcsstr(fileName, extension))
    {
      return true;
    }
  }

  return false;
}

void GetAllImagesInCurrentFolder(vector<wstring>& v)
{
  wchar_t buff[MAX_PATH] = { 0 };

  GetModuleFileName(NULL, buff, MAX_PATH);
  size_t posWack = wcslen(buff) - 1;
  while (buff[posWack] != '\\')
  {
    --posWack;
  }
  buff[posWack] = '\0';
  wcscat_s(buff, L"\\*\0");

  WIN32_FIND_DATA data;
  HANDLE hFind;
  if ((hFind = FindFirstFile(buff, &data)) != INVALID_HANDLE_VALUE) 
  {
    do 
    {
      if (IsFileExtensionInWhitelist(data.cFileName))
      {
        v.emplace_back(data.cFileName);
      }
    } while (FindNextFile(hFind, &data) != 0);

    FindClose(hFind);
  }
}

void PrintAllImageFilenamesFoundInCurrentFolder(const vector<wstring>& allImagesInCurrentFolder)
{
  cout << "Images found in folder:\n";
  for (const auto f : allImagesInCurrentFolder)
  {
    wcout << f << L"\n";
  }
  cout << "\n";
}

void ShowImage(const string& title, const Mat& mat, bool await = false)
{
  imshow(title, mat);

  if (await == true) 
  {
    waitKey(0);
  }
}

void SaveFile(const string& filename, const Mat& mat)
{
  cout << "Saving as " + filename + "\n\n";
  imwrite(filename, mat);
}

void DetectEdges(const Mat& mat)
{
  //AL.

}

void ProcessAllImages(const vector<wstring>& allImagesInCurrentFolder)
{
  for (wstring ws : allImagesInCurrentFolder)
  {
    const string filename(ws.begin(), ws.end());
    const Mat src = imread(filename);

    if (src.data == nullptr)
    {
      cout << "ERROR READING " + filename + "\n";
      continue;
    }

    cout << "Processing " + filename + "\n";

    DetectEdges(src);

#ifdef _DEBUG
    ShowImage(filename, src);
#endif

    //SaveFile(filename + "_processed.jpg", src);
  }
}

void main()
{
#ifdef _DEBUG
  MessageBox(nullptr, L"Attach", L"", 0);
#endif 

  PrintAllSupportedExtensions();

  vector<wstring> allImagesInCurrentFolder;
  GetAllImagesInCurrentFolder(allImagesInCurrentFolder);
  PrintAllImageFilenamesFoundInCurrentFolder(allImagesInCurrentFolder);

  ProcessAllImages(allImagesInCurrentFolder);

  waitKey(0);
}

