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
  int posWack = wcslen(buff) - 1;
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
  vector<wstring> allImagesInCurrentFolder;
  GetAllImagesInCurrentFolder(allImagesInCurrentFolder);

  cout << "Images found in folder:\n\n";
  for (auto f : allImagesInCurrentFolder)
  {
    wcout << f << L"\n";
  }
  cout << "\n";


  for (wstring ws : allImagesInCurrentFolder)
  {
    string s(ws.begin(), ws.end());
    const cv::Mat src = cv::imread(s);
    const int maxLen = max(src.rows, src.cols);
    cv::Mat square = cv::Mat(maxLen, maxLen, src.type());

    if (maxLen == 0)
    {
      continue;
    }

    cout << "converting " + s + "\n";

    const int xDiff = maxLen - src.cols;
    const int yDiff = maxLen - src.rows;

    const cv::Point startPosition = cv::Point(static_cast<int>(xDiff / 2), static_cast<int>(yDiff / 2));

    //Paint square white
    const cv::Vec3b white = cv::Vec3b(255, 255, 255);
    for (int r = 0; r < maxLen; ++ r )
    {
      for (int c = 0; c < maxLen; ++c)
      {
        square.at<cv::Vec3b>(cv::Point(c, r)) = white;
      }
    }

    //Paint in image from starting point
    for (int y = 0; y < src.rows; ++y)
    {
      for (int x = 0; x < src.cols; ++x)
      {
        auto pxSq = square.at<cv::Vec3b>(cv::Point(x + startPosition.x, y + startPosition.y));
        const auto pxSrc = src.at<cv::Vec3b>(cv::Point(x, y));
        pxSq = pxSrc;
      }
    }

    //imshow("", square);

    const string outputName = s + "_sq.jpg";
    cout << "saving as " + outputName + "\n\n";
    imwrite(outputName, square);
  }
  

}

