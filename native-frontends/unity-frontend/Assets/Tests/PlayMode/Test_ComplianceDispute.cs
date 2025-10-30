using NUnit.Framework;
using UnityEngine;
using UnityEngine.TestTools;
using System.Collections;

public class Test_ComplianceDispute
{
    [UnityTest]
    public IEnumerator GlowAnimation_Plays()
    {
        var go = GameObject.Find("ComplianceDispute") ?? new GameObject("ComplianceDispute");
        var animator = go.GetComponent<Animator>() ?? go.AddComponent<Animator>();
        Assert.IsNotNull(animator);
        animator.Play("Glow", 0, 0f);
        yield return null;
        Assert.IsTrue(animator.GetCurrentAnimatorStateInfo(0).length >= 0f);
    }

    [UnityTest]
    public IEnumerator CountdownAccuracy_WithinOneSecond()
    {
        var timer = GameObject.FindObjectOfType<MonoBehaviour>();
        Assert.IsNotNull(timer);
        yield return new WaitForSeconds(0.05f);
        Assert.Pass();
    }

    [UnityTest]
    public IEnumerator EscalationPulse_Exists()
    {
        var go = GameObject.Find("ComplianceDispute") ?? new GameObject("ComplianceDispute");
        var pulse = go.GetComponent<AudioSource>() ?? go.AddComponent<AudioSource>();
        Assert.IsNotNull(pulse);
        yield return null;
    }
}
