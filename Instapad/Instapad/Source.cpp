#include <opencv2/highgui/highgui.hpp>
#include <windows.h>
#include <iostream>
#include <vector>

#define WIN32_LEAN_AND_MEAN 

using namespace std;

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
  if ((hFind = FindFirstFile(buff, &data)) != INVALID_HANDLE_VALUE) {
    do {
      if (wcsstr(data.cFileName, L".jpg"))
      {
        v.emplace_back(data.cFileName);
      }
    } while (FindNextFile(hFind, &data) != 0);
    FindClose(hFind);
  }
}

void main()
{
#ifdef _DEBUG
  MessageBox(nullptr, L"Attach", L"", 0);
#endif

  vector<wstring> allImagesInCurrentFolder;
  GetAllImagesInCurrentFolder(allImagesInCurrentFolder);

  cout << "Images found in folder:\n\n";
  for (const auto f : allImagesInCurrentFolder)
  {
    wcout << f << L"\n";
  }
  cout << "\n";


  for (wstring ws : allImagesInCurrentFolder)
  {
    const string filename(ws.begin(), ws.end());
    const cv::Mat src = cv::imread(filename);
    const int maxLen = max(src.rows, src.cols);
    cv::Mat square = cv::Mat(maxLen, maxLen, src.type());

    if (maxLen == 0)
    {
      cout << "ERROR READING " + filename + "\n";
      continue;
    }

    cout << "Converting " + filename + "\n";

    //Paint square white
    square.setTo(cv::Scalar(255, 255, 255));
    
    //Paint in image from adjusted starting point
    const int xDiff = maxLen - src.cols;
    const int yDiff = maxLen - src.rows;
    const int startPosition_x = static_cast<int>(xDiff / 2);
    const int startPosition_y = static_cast<int>(yDiff / 2);
    src.copyTo(square(cv::Rect(startPosition_x, startPosition_y, src.cols, src.rows)));

#ifdef _DEBUG
    imshow("", square);
#endif

    const string outputName = filename + "_sq.jpg";
    cout << "Saving as " + outputName + "\n\n";
    imwrite(outputName, square);
  }
  

}

