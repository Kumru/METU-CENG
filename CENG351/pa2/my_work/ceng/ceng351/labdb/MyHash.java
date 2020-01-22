package ceng.ceng351.labdb;

import java.lang.Math;
import java.util.LinkedList;
import java.util.ArrayList;

public class MyHash
{
    public class Bucket
    {
        public int localDepth = 1;

        public LinkedList<String> values = new LinkedList<String>();
        Bucket(int localDepth)
        {
            this.localDepth = localDepth;
        }
    }

    public int bucketSize;
    public int globalDepth = 1;
    public ArrayList<Bucket> directory;
    
    public MyHash(int bucketSize)
    {
        this.bucketSize = bucketSize;
        
        directory = new ArrayList<>();

        directory.add(new Bucket(1));
        directory.add(new Bucket(1));
    }

    private int Hash(String ID)
    {
        int numID = Integer.parseInt(ID.substring(1));
        String binID = Integer.toBinaryString((1 << globalDepth) | numID).substring(1);
        String binHash = binID.substring(binID.length()-globalDepth, binID.length());
        return Integer.parseInt(binHash, 2);
    }

    public void Insert(String ID)
    {
        if(Find(ID) != -1)
            return;
        
        int bucketNum = Hash(ID);
        directory.get(bucketNum).values.add(ID);
        if(directory.get(bucketNum).values.size() > bucketSize)
            Split(bucketNum);
    }

    private void Enlarge() 
    {
        int size = directory.size();
        for(int i = 0; i < size; i++)
            directory.add(directory.get(i));
        
        globalDepth++;
    }

    private void Split(int bucketNum)
    {
        LinkedList<String> container =
            new LinkedList<>(directory.get(bucketNum).values);
        directory.get(bucketNum).values.clear();

        int localDepth = ++directory.get(bucketNum).localDepth;
        int peerBucket = bucketNum ^ (1 << (localDepth-1));
        
        if(localDepth > globalDepth)
            Enlarge();
        
        directory.set(peerBucket, new Bucket(localDepth));
        int offset = 1 << localDepth;
        for(int i = peerBucket-offset; i >= 0; i-=offset)
            directory.set(i, directory.get(peerBucket));
        for(int i = peerBucket+offset; i < directory.size(); i+=offset)
            directory.set(i, directory.get(peerBucket));
        
        for (String item : container)
            Insert(item);
    }

    public void Remove(String ID)
    {
        int bucketNum = Hash(ID);
        Bucket bucket = directory.get(bucketNum);
        boolean deletedFlag = bucket.values.remove(ID);

        while(deletedFlag && bucket.values.size() == 0 &&
            globalDepth != 1 && bucket.localDepth != 1)
        {
            if(!MergeBucket(bucketNum))
                break;
            
            CheckAndShrink();

            bucketNum = Hash(ID);
            bucket = directory.get(bucketNum);
        }
    }

    private boolean MergeBucket(int bucketNum)
    {
        int localDepth = directory.get(bucketNum).localDepth;
        int peerBucket = bucketNum ^ (1 << (localDepth-1));

        if(localDepth != directory.get(peerBucket).localDepth)
            return false;

        directory.get(peerBucket).localDepth--;
        
        int offset = 1 << localDepth;
        directory.set(bucketNum, directory.get(peerBucket));

        for(int i = bucketNum-offset; i >= 0; i-=offset)
            directory.set(i, directory.get(peerBucket));
        for(int i = bucketNum+offset; i < directory.size(); i+=offset)
            directory.set(i, directory.get(peerBucket));

        return true;
    }

    private void CheckAndShrink()
    {
        boolean needShrinking = true;
        for (Bucket bucket : directory)
            if(bucket.localDepth == globalDepth)
                return;

        directory.subList(directory.size() >> 1, directory.size()).clear();
        globalDepth--;
    }

    public int Find(String ID)
    {
        int bucketNum = Hash(ID);
        if(directory.get(bucketNum).values.contains(ID))
            return bucketNum;
        return -1;
    }

    String GetBucketLine(int bucketNum)
    {
        String bucketLine = GetBucketString(bucketNum);
        bucketLine += " : [Local depth:";
        bucketLine += Integer.toString(directory.get(bucketNum).localDepth);
        bucketLine += "]";
        
        for (String item : directory.get(bucketNum).values)
            bucketLine += "<" + item + ">";
        
        return bucketLine;
    }

    String GetBucketString(int bucketNum)
    {
        return Integer.toBinaryString((1 << globalDepth) | bucketNum).substring(1);
    }

    void PrintTable()
    {
        System.out.println("Global depth : " + Integer.toString(globalDepth));

        for (int i = 0; i < directory.size(); i++)
            System.out.println(GetBucketLine(i));
    }
}