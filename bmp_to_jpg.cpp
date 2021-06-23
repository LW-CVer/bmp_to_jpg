#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <dirent.h>
#include <vector>
int getFiles(const std::string path, std::vector<std::string>& files, std::string suffix)
{
    int iFileCnt = 0;
    DIR* dirptr = NULL;
    struct dirent* dirp;

    if ((dirptr = opendir(path.c_str())) == NULL)  //打开一个目录
    {
            return 0;
    }
    while ((dirp = readdir(dirptr)) != NULL) {
        if ((dirp->d_type == DT_REG) &&
            0 == (strcmp(strchr(dirp->d_name, '.'),
                         suffix.c_str())))  //判断是否为文件以及文件后缀名
        {
                files.push_back(dirp->d_name);
        }
	 ++iFileCnt;
    }
    closedir(dirptr);

    return iFileCnt;
}
void Jpegcompress(const cv::Mat& src, cv::Mat& dest, int quality)
{
	std::vector<uchar> buff;
	std::vector<int> params;
	/*IMWRITE_JPEG_QUALITY For JPEG, it can be a quality from 0 to 100 
	(the higher is the better). Default value is 95 */
	params.push_back(cv::IMWRITE_JPEG_QUALITY);
	params.push_back(quality);
	//将图像压缩编码到缓冲流区域
	cv::imencode(".jpg", src, buff, params);
	//将压缩后的缓冲流内容解码为Mat，进行后续的处理
	dest = cv::imdecode(buff, 1);

}

int main(int argc,char* argv[])
{
    std::string dirName = argv[1];
    int quality=std::atoi(argv[2]);
    std::vector<std::string> files;
    getFiles(dirName,files,".bmp");
    for(auto file:files){
        cv::Mat src = cv::imread(dirName+file);
        if (src.empty())
        {
            std::cerr<<" image open error!\n";
            return 0;
        }
        cv::Mat dest;
        //质量等级为设为85
        //Jpegcompress(src,dest,quality);   
        cv::resize(src,dest,cv::Size(960,540));
        //cv2.IMWRITE_JPEG_QUALITY类型为 long ,必须转换成 int
        cv::imwrite("./result/"+file,dest);
    }

    return 0;
}
