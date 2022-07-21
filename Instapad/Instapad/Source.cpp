#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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
  namedWindow(title, WINDOW_NORMAL);
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

void DetectEdges(const Mat& src)
{
  //Graycsale
  Mat img_gray;
  cvtColor(src, img_gray, COLOR_BGR2GRAY);
  
  //Blur
  Mat img_blur;
  GaussianBlur(img_gray, img_blur, Size(3,3), 0);
  
  //Edge detection
  Mat edges;
  Canny(img_blur, edges, 100, 200, 3, false);
  //Sobel(img_blur, edges, CV_64F, 1, 0, 1);

#ifdef _DEBUG
  // Display canny edge detected image
  ShowImage(to_string(rand()), edges);
#endif
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

#ifdef _DEBUG
    ShowImage(filename, src);
#endif

    DetectEdges(src);

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

