@echo off
cd /d "c:\Users\Lenovo\OneDrive\Desktop\BTECH\Btech sem 2\Advance DSA\project chechker"

echo Initializing Git repository...
git init

echo Adding files...
git add .

echo Committing files...
git commit -m "Initial commit of project checker"

echo Setting main branch...
git branch -M main

echo Adding remote origin...
git remote add origin https://github.com/sankalpjaiswal013-ops/Plagiarism-checker-ads.git

echo Pushing to GitHub...
git push -u origin main

echo.
echo Finished! Check the messages above for any errors.
pause
