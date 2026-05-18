mkdir -p linux_practice/docs
mkdir -p linux_practice/backup
touch linux_practice/docs/readme.txt
touch linux_practice/docs/notes.log
touch linux_practice/docs/temp.tmp
rm linux_practice/docs/temp.tmp
mv linux_practice/docs/notes.log linux_practice/docs/daily_report.txt
echo "Project Status: Active" > linux_practice/docs/daily_report.txt
echo "$(date)">>linux_practice/docs/daily_report.txt
cp linux_practice/docs/*.txt linux_practice/backup/
for file in linux_practice/backup/*; do
    chmod 444 "$file"
    filename=$(basename "$file")
    echo "Archive Complete. File [${filename}] is now read-only."
done
