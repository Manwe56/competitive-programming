package competitive.programming.genetic;

/**
 * @author Manwe
 * 
 * Interface that evaluate a candidate. During the selection phase, only the candidates with the highest score will be retained
 *
 * @param <Genotype>
 *  The class representing one candidate
 */
public interface FitnessFunction<Genotype> {
    /**
     * @param genotype
     * the genotype to be evaluated
     * @return
     * the double value representing the quality of a candidate. The higher the better.
     */
    double evaluate(Genotype genotype);
}
