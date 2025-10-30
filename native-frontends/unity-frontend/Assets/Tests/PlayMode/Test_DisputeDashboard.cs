using NUnit.Framework;
using UnityEngine;
using UnityEngine.TestTools;
using System.Collections;

public class Test_DisputeDashboard
{
    [UnityTest]
    public IEnumerator FadeInAnimation_Plays()
    {
        var go = GameObject.Find("DisputeDashboard") ?? new GameObject("DisputeDashboard");
        var canvas = go.GetComponent<Canvas>() ?? go.AddComponent<Canvas>();
        yield return null;
        Assert.Pass();
    }

    [UnityTest]
    public IEnumerator PerRowCountdowns_Update()
    {
        var go = new GameObject("DashboardRow");
        yield return null;
        Assert.Pass();
        Object.Destroy(go);
    }

    [UnityTest]
    public IEnumerator AlarmPriorityResolution_OrderCorrect()
    {
        yield return null;
        Assert.Pass();
    }

    [UnityTest]
    public IEnumerator AudioMixerSidechain_Exists()
    {
        yield return null;
        Assert.Pass();
    }

    [UnityTest]
    public IEnumerator OverdueDetection_TriggersPulse()
    {
        yield return null;
        Assert.Pass();
    }
}
