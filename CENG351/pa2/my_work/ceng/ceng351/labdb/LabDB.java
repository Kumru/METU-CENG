package ceng.ceng351.labdb;

import ceng.ceng351.labdb.MyHash;

public class LabDB
{
    public MyHash lab;

    public LabDB(int bucketSize)
    {
        lab = new MyHash(bucketSize);
    }

    public void enter(String studentID)
    {
        lab.Insert(studentID);
    }

    public void leave(String studentID)
    {
        lab.Remove(studentID);
    }

    public String search(String studentID)
    {
        int bucketNum = lab.Find(studentID);
        if(bucketNum != -1)
        {
            return lab.GetBucketString(bucketNum);
        }
        
        return "-1";
    }

    public void printLab()
    {
        lab.PrintTable();
    }
}
