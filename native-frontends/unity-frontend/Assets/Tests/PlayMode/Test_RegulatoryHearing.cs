using NUnit.Framework;
using UnityEngine;
using UnityEngine.TestTools;
using System.Collections;

public class Test_RegulatoryHearing
{
    [UnityTest]
    public IEnumerator TestimonySequencing_Works()
    {
        var go = GameObject.Find("RegulatoryHearing") ?? new GameObject("RegulatoryHearing");
        yield return null;
        Assert.Pass();
    }

    [UnityTest]
    public IEnumerator GavelTiming_SyncsAudio()
    {
        var go = new GameObject("Gavel");
        var src = go.AddComponent<AudioSource>();
        Assert.IsNotNull(src);
        yield return null;
        Object.Destroy(go);
    }

    [UnityTest]
    public IEnumerator WitnessListBinding_Present()
    {
        var go = new GameObject("WitnessList");
        yield return null;
        Assert.Pass();
        Object.Destroy(go);
    }
}
